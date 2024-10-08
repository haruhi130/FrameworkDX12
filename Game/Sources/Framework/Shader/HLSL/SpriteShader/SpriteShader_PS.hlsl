#include "inc_SpriteShader.hlsli"

Texture2D g_inputTex : register(t0);

SamplerState g_ss : register(s0);

float4 main(VSOutput In) : SV_TARGET
{
    float4 color = g_inputTex.Sample(g_ss, In.UV);
	
    if (color.a < 0.1f) discard;
	
	return color * g_color;
}