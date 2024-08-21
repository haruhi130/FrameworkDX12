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