#include "inc_ModelShader.hlsli"
#include "../inc_Common.hlsli"

Texture2D g_diffuseTex : register(t0);              // Diffuseテクスチャ
Texture2D g_emissiveTex : register(t1);             // Emissiveテクスチャ
Texture2D g_metallicRoughnessTex : register(t2);    // MetallicRoughnessテクスチャ
Texture2D g_normalTex : register(t3);               // Normalテクスチャ

Texture2D<float> g_shadowTex : register(t4);        // シャドウマップ

SamplerState g_ssWP : register(s0); // Wrap  : Point
SamplerState g_ssCP : register(s1); // Clamp : Point
SamplerState g_ssWL : register(s2); // Wrap  : Linear
SamplerState g_ssCL : register(s3); // Clamp : Linear

SamplerComparisonState g_ssCmp : register(s4); // 補間用比較機能付き

// bayerMatrix
static const int bayerMatrix[4][4] =
{
    {  0, 8, 2,10 },
    { 12, 4,14, 6 },
    {  3,11, 1, 9 },
    { 15, 7,13, 5 }
};

float BlinnPhong(float3 lightDir, float3 vCam, float3 normal, float specPower)
{
    float3 H = normalize(-lightDir + vCam);
    float NdotH = saturate(dot(normal, H)); // カメラの角度差(0～1)
    float spec = pow(NdotH, specPower);
    
    // 正規化Blinn-Phong
    return spec * ((specPower + 2) / (2 * 3.14159265358979f));
}

float4 main(VSOutput In) : SV_TARGET
{
    // 材質色
    float4 baseColor = g_diffuseTex.Sample(g_ssWL, In.uv) * g_baseCol * In.color;
    
    // Alphaテスト
    if (baseColor.a < 0.05f)
    {
        discard;
    }
    
    // カメラへの方向
    float3 vCam = normalize(g_camPos - In.wPos);
    
    // AlphaDither
    if(g_isDitherEnable)
    {
        // ピクセル算出
        int x = (int) fmod(In.svPos.x, 5.0);
        int y = (int) fmod(In.svPos.y, 5.0);
        
        // bayerMatrixから0～1のしきい値を算出
        float dither = bayerMatrix[y][x] / 16.0;
        
        // AlphaDitherを行うカメラからの距離
        float ditherDist = 1.1f;
        
        // ditherDist分のピクセルが対象
        float range = max(0, In.wvPos.z - ditherDist);
        
        // 割合算出
        float rate = 1 - min(1, range);
        
        // 0未満ならピクセル破棄
        clip(dither - 1 * rate);
    }
    
    // 法線マップから法線べクトルを取得
    float3 vNormal = g_normalTex.Sample(g_ssWL, In.uv).rgb;
    
    // UV座標から射影座標へ変換
    vNormal = vNormal * 2.0 - 1.0;
    
    // 3種の法線から行列を作成
    row_major float3x3 mTBN =
    {
        normalize(In.wT),
        normalize(In.wB),
        normalize(In.wN),
    };
    
    // 法線ベクトルをピクセル空間へ変換
    vNormal = mul(vNormal, mTBN);
    
    // 正規化
    vNormal = normalize(vNormal);
    
    // 金属性・粗さ色
    float4 mr = g_metallicRoughnessTex.Sample(g_ssWL, In.uv);
    
    // 金属性
    float metallic = g_metallic * mr.b;
    
    // 粗さ
    float roughness = g_roughness * mr.g;
    // 滑らかさ
    float smoothness = 1.0 - roughness;
    float specPower = pow(2, 11 * smoothness); // 1～2048
    
    // 最終的に出力される色
    float3 outColor = 0;
    
    //------------------------------------------
    // シャドウマッピング
    //------------------------------------------
    float shadow = 1;
    
    float4 liPos = In.tPos;
    liPos.xyz /= liPos.w;

    float bias = 0.003f;
    
    // 深度マップの範囲内かチェック
    if (abs(liPos.x) <= 1 && abs(liPos.y) <= 1 && liPos.z <= 1)
    {
        // 射影座標からUV座標へ変換
        float2 uv = (liPos.xy * float2(1, -1)) * 0.5 + 0.5;
        // ライト方向からのカメラ距離(シャドウアクネ対策)
        float z = liPos.z - bias;

        // 画像サイズからテクセルサイズを計算        
        float w, h;
        g_shadowTex.GetDimensions(w, h);
        float tw = 1.0 / w;
        float th = 1.0 / h;
        
        // UVの周辺3x3を判定し平均値を計算
        shadow = 0;
        for (int y = -1; y <= 1; y++)
        {
            for (int x = -1; x <= 1; x++)
            {
                shadow += g_shadowTex.SampleCmpLevelZero(g_ssCmp, uv + float2(x * tw, y * th), z);
            }
        }
        shadow *= 0.11;
    }
    
    //------------------------------------------
    // ライティング
    //------------------------------------------
    // 光の計算を行うか
    if (g_isUseLight)
    {
        // 材質の拡散色   非金属ほど材質の色になり金属ほど拡散色は無くなる
        float3 baseDiffuse = lerp(baseColor.rgb, float3(0, 0, 0), metallic);
        // 材質の反射色   非金属ほど光の色を反射し金属ほど材質の色が乗る
        float3 baseSpecular = lerp(0.04, baseColor.rgb, metallic);
        
        // Diffuse
        {
            // 光の方向と法線の方向の角度差
            float Dot = dot(-g_DL_Dir, vNormal);
            // マイナス値は0
            float DiffusePow = saturate(Dot);
            
            // 正規化Lambert
            DiffusePow /= 3.14159265358979f;
        
            // 光の色 * 材質の拡散色 * 影
            outColor += (g_DL_Color * DiffusePow) * baseDiffuse * shadow;
        }
    
        // Specular
        {
            // 反射した光の強さ
            float spec = BlinnPhong(g_DL_Dir, vCam, vNormal, specPower);
        
            // 光の色 * 反射光の強さ * 材質の反射色 * 調整値 * 影
            outColor += (g_DL_Color * spec) * baseSpecular * 0.5f * shadow;
        }
        
        // Ambient
        {
            // 全体の明度
            float toBright = saturate(g_AL_Power);
            outColor += toBright * baseColor.rgb;
        }
    }
    else
    {
        // 光の計算をしない場合材質色を返す
        outColor = baseColor.rgb;
    }
    
    //------------------------------------------
	// 出力
	//------------------------------------------
    return float4(outColor, baseColor.a);
}