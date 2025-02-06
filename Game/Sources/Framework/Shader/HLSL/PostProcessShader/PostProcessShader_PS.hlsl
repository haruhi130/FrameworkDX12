#include "inc_PostProcessShader.hlsli"

Texture2D<float4> tex : register(t0);

SamplerState smp : register(s2);

float4 main(Output In) : SV_TARGET
{
    float4 base = tex.Sample(smp, In.UV);
    
    return base;
}