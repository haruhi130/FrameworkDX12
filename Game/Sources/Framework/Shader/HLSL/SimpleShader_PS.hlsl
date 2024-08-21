#include "inc_SimpleShader.hlsli"
#include "inc_Common.hlsli"

Texture2D g_diffuseTex : register(t0); // Diffuse�e�N�X�`��
Texture2D g_normalTex : register(t1); // Normal�e�N�X�`��
Texture2D g_MetallicRoughnessTex : register(t2); // MetallicRoughness�e�N�X�`��
Texture2D g_EmissiveTex : register(t3); // Emissive�e�N�X�`��

SamplerState g_ss : register(s0);

float4 main(VSOutput In) : SV_TARGET
{
    float4 color = g_diffuseTex.Sample(g_ss, In.uv);
    return color;
}