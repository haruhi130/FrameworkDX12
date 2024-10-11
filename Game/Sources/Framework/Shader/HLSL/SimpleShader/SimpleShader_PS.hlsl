#include "inc_SimpleShader.hlsli"
#include "../inc_Common.hlsli"

Texture2D g_diffuseTex : register(t0); // Diffuseテクスチャ
Texture2D g_normalTex : register(t1); // Normalテクスチャ
Texture2D g_metallicRoughnessTex : register(t2); // MetallicRoughnessテクスチャ
Texture2D g_emissiveTex : register(t3); // Emissiveテクスチャ

SamplerState g_ssWP : register(s0);
SamplerState g_ssCP : register(s1);
SamplerState g_ssWL : register(s2);
SamplerState g_ssCL : register(s3);

float BlinnPhong(float3 lightDir,float3 vCam,float3 normal,float specPower)
{
    float3 H = normalize(-lightDir + vCam);
    float NdotH = saturate(dot(normal, H));
    float spec = pow(NdotH, specPower);
    
    return spec * ((specPower + 2) / (2 * 3.14159265358979f));
}

float4 main(VSOutput In) : SV_TARGET
{
    float4 baseColor = g_diffuseTex.Sample(g_ssWL, In.uv) * g_baseCol * In.color;
    
    float3 outColor = 0;
    
    if(g_isUseLight)
    {
        float3 vNormal = normalize(In.normal);
        float Dot = dot(vNormal, -g_DL_Dir);
        
        float DiffusePow = saturate(Dot);
        DiffusePow /= 3.14159265358979f;
        
        outColor += baseColor.rgb * g_DL_Color * baseColor.a * DiffusePow;
    
        {
            outColor += baseColor.rgb * g_AL_Power;
        }
    }
    else
    {
        return baseColor;
    }
    
    return float4(outColor, baseColor.a);
}