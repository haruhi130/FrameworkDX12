#include "inc_SimpleShader.hlsli"
#include "inc_Common.hlsli"

VSOutput main( 
float4 pos : POSITION,float2 uv : TEXCOORD,float3 normal : NORMAL,
float4 color : COLOR,float3 tangent :TANGENT)
{
    VSOutput Out;
    Out.pos = mul(pos, g_mWorld);
    Out.pos = mul(Out.pos, g_mView);
    Out.pos = mul(Out.pos, g_mProj);
    Out.uv = uv;
    Out.color = color;
    Out.normal = normal;
    Out.tangent = tangent;
	return Out;
}