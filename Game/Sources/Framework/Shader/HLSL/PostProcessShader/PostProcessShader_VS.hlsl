#include "inc_PostProcessShader.hlsli"

Output main( float4 pos : POSITION,float2 uv:TEXCOORD )
{
    Output Out;
	
    Out.svPos = pos;
    Out.uv = uv;
	
	return Out;
}