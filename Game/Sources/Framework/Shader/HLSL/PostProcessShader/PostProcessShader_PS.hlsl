#include "inc_PostProcessShader.hlsli"

Texture2D<float4> tex : register(t0);

SamplerState smp : register(s2);

float4 main(Output In) : SV_TARGET
{
    float4 base = tex.Sample(smp, In.uv);
    
    //float w, h, levels;
    //tex.GetDimensions(0, w, h, levels);
    
    //float4 ret = float4(0, 0, 0, 0);
    //float dx = 1.0f / w;
    //float dy = 1.0f / h;
    
    //ret += tex.Sample(smp, In.uv + float2(0, -2 * dy)) * -1;
    //ret += tex.Sample(smp, In.uv + float2(-2 * dx, 0)) * -1;
    //ret += tex.Sample(smp, In.uv) * 4;
    //ret += tex.Sample(smp, In.uv + float2(2 * dx, 0)) * -1;
    //ret += tex.Sample(smp, In.uv + float2(0, 2 * dy)) * -1;
    
    //float Y = dot(ret.rgb, float3(0.299, 0.587, 0.114));
 
    //Y = pow(1.0f - Y, 3.0f);
    //Y = step(0.1, Y);
    
    //return float4(Y, Y, Y, base.a);
    
    return base;
}