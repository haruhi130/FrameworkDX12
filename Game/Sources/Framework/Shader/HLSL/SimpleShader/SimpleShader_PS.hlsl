#include "inc_SimpleShader.hlsli"
#include "../inc_Common.hlsli"

Texture2D g_diffuseTex : register(t0); // Diffuseテクスチャ
Texture2D g_normalTex : register(t1); // Normalテクスチャ
Texture2D g_MetallicRoughnessTex : register(t2); // MetallicRoughnessテクスチャ
Texture2D g_EmissiveTex : register(t3); // Emissiveテクスチャ

SamplerState g_ssWP : register(s0);
SamplerState g_ssCP : register(s1);
SamplerState g_ssWL : register(s2);
SamplerState g_ssCL : register(s3);

float4 main(VSOutput In) : SV_TARGET
{
    float4 color = g_diffuseTex.Sample(g_ssWL, In.uv);
    return color;
}