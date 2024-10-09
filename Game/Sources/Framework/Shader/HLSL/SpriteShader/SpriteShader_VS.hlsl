#include "inc_SpriteShader.hlsli"

VSOutput main( float4 pos : POSITION,float2 uv : TEXCOORD)
{
    VSOutput Out;
    
    Out.Pos = mul(pos,g_mTransform);
    Out.Pos = mul(Out.Pos,g_mProj);
    
    Out.UV = uv;
    
	return Out;
}