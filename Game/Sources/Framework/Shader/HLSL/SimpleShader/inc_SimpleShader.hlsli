struct VSOutput
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 color : COLOR;
};

cbuffer cbWorld : register(b1)
{
    row_major matrix g_mWorld;
}

cbuffer cbObject : register(b2)
{
    int g_isSkinMeshObj;
}

cbuffer cbBones : register(b3)
{
    row_major float4x4 g_mBones[300];
}