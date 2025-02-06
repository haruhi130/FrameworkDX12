#include "inc_SpriteShader.hlsli"

Texture2D g_inputTex : register(t0);

SamplerState g_ssCP : register(s1);

float4 main(VSOutput In) : SV_TARGET
{
    float4 color = g_inputTex.Sample(g_ssCP, In.UV);
	
	// AlphaƒeƒXƒg
    if (color.a < 0.1f) discard;
	
	return color * g_Color;
}