struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 color : COLOR;
};

cbuffer cbViewProj : register(b0)
{
    row_major matrix g_mView;
    row_major matrix g_mProj;
}

cbuffer cbWorld : register(b1)
{
    row_major matrix g_mWorld;
}