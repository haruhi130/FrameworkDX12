// 出力データ
struct VSOutput
{
    float4 svPos    : SV_Position;  // 射影座標
    float3 wPos     : TEXCOORD0;    // ワールド3D座標
    float2 uv       : TEXCOORD1;    // UV座標
    float3 tangent  : TANGENT;      // タンジェント値
    float4 color    : COLOR;        // 色
    
    float3 wN       : TEXCOORD2;    // ワールド法線
    float3 wT       : TEXCOORD3;    // ワールド接線
    float3 wB       : TEXCOORD4;    // ワールド従法線
    float4 tPos     : TPOS;         // ライトカメラ座標
    float4 pos      : Position;     // 標準座標
};

// ワールド変換行列
cbuffer cbWorld : register(b0)
{
    row_major float4x4 g_mWorld;
}

// スキンメッシュ判別
cbuffer cbObject : register(b1)
{
    int g_isSkinMeshObj;
}

// スキンメッシュボーン
cbuffer cbBones : register(b2)
{
    row_major float4x4 g_mBones[128];
}

// マテリアル色
cbuffer cbMaterial : register(b3)
{
    float4 g_baseCol;   // ベース色
    float3 g_emissive;  // 自己発光色
    float g_metallic;   // 金属度
    float g_roughness;  // 粗さ
}