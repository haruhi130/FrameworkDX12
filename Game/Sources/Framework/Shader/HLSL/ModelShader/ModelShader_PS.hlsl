#include "inc_ModelShader.hlsli"
#include "../inc_Common.hlsli"

Texture2D g_diffuseTex : register(t0);              // Diffuse�e�N�X�`��
Texture2D g_emissiveTex : register(t1);             // Emissive�e�N�X�`��
Texture2D g_metallicRoughnessTex : register(t2);    // MetallicRoughness�e�N�X�`��
Texture2D g_normalTex : register(t3);               // Normal�e�N�X�`��

SamplerState g_ssWP : register(s0); // Wrap  : Point
SamplerState g_ssCP : register(s1); // Clamp : Point
SamplerState g_ssWL : register(s2); // Wrap  : Linear
SamplerState g_ssCL : register(s3); // Clamp : Linear

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
    float smoothness = 1.0 - roughness * roughness;
    float specPower = pow(2, 11 * smoothness);
    
    // �o�͂���F
    float3 outColor = 0;
    
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
            float Dot = dot(-g_DL_Dir,vNormal);
            // �}�C�i�X�l��0
            float DiffusePow = saturate(Dot);
            
            // ���K��Lambert
            DiffusePow /= 3.14159265358979f;
        
            // ���̐F * �ގ��̊g�U�F
            outColor += (g_DL_Color * DiffusePow) * baseDiffuse * baseColor.a;
        }
    
        // Specular
        {
            // ���˂������̋���
            float spec = BlinnPhong(g_DL_Dir, vCam, vNormal, specPower);
        
            // ���̐F * ���ˌ��̋��� * �ގ��̔��ːF
            outColor += (g_DL_Color * spec) * baseSpecular * baseColor.a;
        }
        
        // Ambient
        {
            // �S�̖̂��x
            outColor += g_AL_Power * baseColor.rgb * baseColor.a;
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