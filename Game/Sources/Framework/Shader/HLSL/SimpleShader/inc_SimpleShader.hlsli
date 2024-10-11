struct VSOutput
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 color : COLOR;
    
    float3 wN : TEXCOORD2;
    float3 wT : TEXCOORD3;
    float3 wB : TEXCOORD4;
    float3 wPos : TEXCOORD5;
};

cbuffer cbWorld : register(b1)
{
    row_major float4x4 g_mWorld;
}

cbuffer cbObject : register(b2)
{
    int g_isSkinMeshObj;
}

cbuffer cbBones : register(b3)
{
    row_major float4x4 g_mBones[300];
}

cbuffer cbMaterial : register(b4)
{
    float4 g_baseCol;
    float3 g_emissive;
    float g_metallic;
    float g_roughness;
}