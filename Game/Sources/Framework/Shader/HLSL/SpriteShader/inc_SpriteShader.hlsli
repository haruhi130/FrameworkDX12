struct VSOutput
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};

cbuffer cbSprite :register(b0)
{
    row_major float4x4 g_mTransform;
    float4 g_Color;
};

cbuffer cbProjection : register(b1)
{
    row_major float4x4 g_mProj;
}