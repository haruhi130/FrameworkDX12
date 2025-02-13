// 出力データ
struct VSOutput
{
    float4 svPos    : SV_Position;  // 射影座標
    float3 wPos     : TEXCOORD0;    // ワールド3D座標
    float2 UV       : TEXCOORD1;    // UV座標
    float3 Tangent  : TANGENT;      // タンジェント値
    float4 Color    : COLOR;        // 色
    
    float3 wN       : TEXCOORD2;    // ワールド法線
    float3 wT       : TEXCOORD3;    // ワールド接線
    float3 wB       : TEXCOORD4;    // ワールド従法線
    
    float4 Pos      : Position;     // 標準座標
    float3 wvPos    : TEXCOORD5;    
};

// ワールド変換行列
cbuffer cbWorld : register(b0)
{
    row_major float4x4 g_mWorld;
}

// オブジェクト情報
cbuffer cbObject : register(b1)
{
    int g_IsSkinMeshObj;    // スキンメッシュ判別
    
    int g_IsDitherEnable;   // アルファディザ有効
    
    int g_LimLightEnable;   // リムライト有効
    float g_LimLightLevel;  // リムライトの乗数(強さ)
    float3 g_LimLightColor; // リムライトの色
}

// ボーン情報
cbuffer cbBones : register(b2)
{
    row_major float4x4 g_mBones[300];
}

// マテリアル色
cbuffer cbMaterial : register(b3)
{
    float4 g_BaseCol;   // ベース色
    float3 g_Emissive;  // 自己発光色
    float g_Metallic;   // 金属度
    float g_Roughness;  // 粗さ
}