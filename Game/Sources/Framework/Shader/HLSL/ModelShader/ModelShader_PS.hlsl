#include "inc_ModelShader.hlsli"
#include "../inc_Common.hlsli"

Texture2D<float4> g_diffuseTex : register(t0);              // Diffuse�e�N�X�`��
Texture2D<float4> g_emissiveTex : register(t1);             // Emissive�e�N�X�`��
Texture2D<float4> g_metallicRoughnessTex : register(t2);    // MetallicRoughness�e�N�X�`��
Texture2D<float4> g_normalTex : register(t3);               // Normal�e�N�X�`��

Texture2D<float> g_shadowTex : register(t4);

SamplerState g_ssWP : register(s0); // Wrap  : Point
SamplerState g_ssCP : register(s1); // Clamp : Point
SamplerState g_ssWL : register(s2); // Wrap  : Linear
SamplerState g_ssCL : register(s3); // Clamp : Linear

SamplerComparisonState g_ssCmp : register(s4);

float BlinnPhong(float3 lightDir, float3 vCam, float3 normal, float specPower)
{
    float3 H = normalize(-lightDir + vCam);
    float NdotH = saturate(dot(normal, H)); // �J�����̊p�x��(0�`1)
    float spec = pow(NdotH, specPower);
    
    // ���K��Blinn-Phong
    return spec * ((specPower + 2) / (2 * 3.14159265358979f));
}

float4 main(VSOutput In) : SV_TARGET
{
    //float dep = g_shadowTex.Sample(g_ssCP, In.uv);
    //return float4(dep, dep, dep, 1);
    
    // �J�����ւ̕���
    float3 vCam = normalize(g_camPos - In.wPos);
    
    // �@���}�b�v����@���׃N�g�����擾
    float3 vNormal = g_normalTex.Sample(g_ssWL, In.uv).rgb;
    
    // UV���W����ˉe���W�֕ϊ�
    vNormal = vNormal * 2.0 - 1.0;
    
    // 3��̖@������s����쐬
    row_major float3x3 mTBN =
    {
        normalize(In.wT),
        normalize(In.wB),
        normalize(In.wN),
    };
    
    // �@���x�N�g�����s�N�Z����Ԃ֕ϊ�
    vNormal = mul(vNormal, mTBN);
    
    // ���K��
    vNormal = normalize(vNormal);
    
    // �ގ��F
    float4 baseColor = g_diffuseTex.Sample(g_ssWL, In.uv) * g_baseCol * In.color;
    
    float4 mr = g_metallicRoughnessTex.Sample(g_ssWL, In.uv);
    
    // ������
    float metallic = g_metallic * mr.b;
    
    // �e��
    float roughness = g_roughness * mr.g;
    // ���炩��
    float smoothness = 1.0 - roughness;
    float specPower = pow(2, 11 * smoothness);
    
    // �o�͂���F
    float3 outColor = 0;
    
    float shadow = 1;
    
    //float3 vp = In.tPos.xyz / In.tPos.w;
    //float2 shadowUV = (vp.xy + float2(1, -1)) * float2(0.5, -0.5);
    //float depthLight = g_shadowTex.SampleCmp(g_ssCmp, shadowUV, vp.z - 0.005f);
    //shadow = lerp(0.5f, 1.0f, depthLight);
    
    
    float4 liPos = In.tPos;
    liPos.xyz /= liPos.w;
    
    if (abs(liPos.x) <= 1 && abs(liPos.y) <= 1 && liPos.z <= 1)
    {
        float2 uv = (liPos.xy * float2(1, -1)) * 0.5 + 0.5;
        float z = liPos.z - 0.004f;
        
        float w, h;
        g_shadowTex.GetDimensions(w, h);
        float tw = 1.0 / w;
        float th = 1.0 / h;
        
        shadow = 0;
        for (int y = -1; y <= 1; y++)
        {
            for (int x = -1; x <= 1; x++)
            {
                shadow += g_shadowTex.SampleCmpLevelZero(g_ssCmp, uv + float2(x * tw, y * th), z);
            }
        }
        shadow *= 0.11;
    }
    
    //------------------------------------------
    // ���C�e�B���O
    //------------------------------------------
    // ���̌v�Z���s����
    if (g_isUseLight)
    {
        // �ގ��̊g�U�F   ������قǍގ��̐F�ɂȂ�����قǊg�U�F�͖����Ȃ�
        float3 baseDiffuse = lerp(baseColor.rgb, float3(0, 0, 0), metallic);
        // �ގ��̔��ːF   ������قǌ��̐F�𔽎˂������قǍގ��̐F�����
        float3 baseSpecular = lerp(0.04, baseColor.rgb, metallic);
        
        // Diffuse
        {
            // ���̕����Ɩ@���̕����̊p�x�������̋���
            float Dot = dot(-g_DL_Dir, vNormal);
            // �}�C�i�X�l��0
            float DiffusePow = saturate(Dot);
            
            // ���K��Lambert
            DiffusePow /= 3.14159265358979f;
        
            // ���̐F * �ގ��̊g�U�F
            outColor += (g_DL_Color * DiffusePow) * baseDiffuse * baseColor.a * shadow;
        }
    
        // Specular
        {
            // ���˂������̋���
            float spec = BlinnPhong(g_DL_Dir, vCam, vNormal, specPower);
        
            // ���̐F * ���ˌ��̋��� * �ގ��̔��ːF
            outColor += (g_DL_Color * spec) * baseSpecular * baseColor.a * shadow;
        }
        
        // Ambient
        {
            // �S�̖̂��x
            float toBright = saturate(g_AL_Power);
            outColor += toBright * baseColor.rgb * baseColor.a;
        }
    }
    else
    {
        // ���̌v�Z�����Ȃ��ꍇ�ގ��F��Ԃ�
        outColor = baseColor.rgb;
    }
    
    //------------------------------------------
	// �o��
	//------------------------------------------
    return float4(outColor, baseColor.a);
}