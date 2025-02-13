#include "inc_ModelShader.hlsli"
#include "../inc_Common.hlsli"

Texture2D g_diffuseTex : register(t0); // Diffuse�e�N�X�`��
Texture2D g_emissiveTex : register(t1); // Emissive�e�N�X�`��
Texture2D g_metallicRoughnessTex : register(t2); // MetallicRoughness�e�N�X�`��
Texture2D g_normalTex : register(t3); // Normal�e�N�X�`��

Texture2D<float> g_shadowTex : register(t4); // �V���h�E�}�b�v

SamplerState g_ssWP : register(s0); // Wrap  : Point
SamplerState g_ssCP : register(s1); // Clamp : Point
SamplerState g_ssWL : register(s2); // Wrap  : Linear
SamplerState g_ssCL : register(s3); // Clamp : Linear

SamplerComparisonState g_ssCmp : register(s4); // ��ԗp��r�@�\�t��

// bayerMatrix
static const int bayerMatrix[4][4] =
{
    { 0, 8, 2, 10 },
    { 12, 4, 14, 6 },
    { 3, 11, 1, 9 },
    { 15, 7, 13, 5 }
};

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
    // �ގ��F
    float4 baseColor = g_diffuseTex.Sample(g_ssWL, In.UV) * g_BaseCol * In.Color;
    
    // Alpha�e�X�g
    if (baseColor.a < 0.05f)
    {
        discard;
    }
    
    // �J�����ւ̕���
    float3 vCam = normalize(g_CamPos - In.wPos);
    
    // AlphaDither
    if (g_IsDitherEnable)
    {
        // �s�N�Z���Z�o
        int x = (int) fmod(In.svPos.x, 4.0);
        int y = (int) fmod(In.svPos.y, 4.0);
        
        // bayerMatrix����0�`1�̂������l���Z�o
        float dither = bayerMatrix[y][x] / 16.0;
        
        // AlphaDither���s���J��������̋���
        float ditherDist = 0.5f;
        
        // ditherDist���̃s�N�Z�����Ώ�
        float range = max(0, In.wvPos.z - ditherDist);
        
        // �����Z�o
        float rate = 1 - min(1, range);
        
        // 0�����Ȃ�s�N�Z���j��
        clip(dither - 1 * rate);
    }
    
    // �@���}�b�v����@���׃N�g�����擾
    float3 vNormal = g_normalTex.Sample(g_ssWL, In.UV).rgb;
    
    // UV���W����ˉe���W�֕ϊ�
    vNormal = vNormal * 2.0 - 1.0;
    
    {
        // 3��̖@������s����쐬
        row_major float3x3 mTBN =
        {
            normalize(In.wT),
            normalize(In.wB),
            normalize(In.wN),
        };
    
        // �@���x�N�g�����s�N�Z����Ԃ֕ϊ�
        vNormal = mul(vNormal, mTBN);
    }
    
    // ���K��
    vNormal = normalize(vNormal);
    
    // �������E�e���F
    float4 mr = g_metallicRoughnessTex.Sample(g_ssWL, In.UV);
    
    // ������
    float metallic = mr.b * g_Metallic;
    
    // �e��
    float roughness = mr.g * g_Roughness;
    // ���炩��
    float smoothness = 1.0 - roughness;
    float specPower = pow(2, 11 * smoothness); // 1�`2048
    
    // �ŏI�I�ɏo�͂����F
    float3 outColor = 0;
    
    //------------------------------------------
    // �V���h�E�}�b�s���O
    //------------------------------------------
    float shadow = 1;
    
    float4 liPos = mul(float4(In.wPos, 1), g_DL_mVP);
    liPos.xyz /= liPos.w;

    float bias = 0.004f;
    
    // �[�x�}�b�v�͈͓̔����`�F�b�N
    if (abs(liPos.x) <= 1 && abs(liPos.y) <= 1 && liPos.z <= 1)
    {
        // �ˉe���W����UV���W�֕ϊ�
        float2 uv = liPos.xy * float2(1, -1) * 0.5 + 0.5;
        // ���C�g��������̃J��������(�V���h�E�A�N�l�΍�)
        float z = liPos.z - bias;

        // �摜�T�C�Y����e�N�Z���T�C�Y���v�Z        
        float w, h;
        g_shadowTex.GetDimensions(w, h);
        float tw = 1.0 / w;
        float th = 1.0 / h;
        
        // UV�̎���3x3�𔻒肵���ϒl���v�Z
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
    if (g_IsUseLight)
    {
        // �ގ��̊g�U�F   ������قǍގ��̐F�ɂȂ�����قǊg�U�F�͖����Ȃ�
        const float3 baseDiffuse = lerp(baseColor.rgb, float3(0, 0, 0), metallic);
        // �ގ��̔��ːF   ������قǌ��̐F�𔽎˂������قǍގ��̐F�����
        const float3 baseSpecular = lerp(0.04, baseColor.rgb, metallic);
        
        // Diffuse
        {
            // ���̕����Ɩ@���̕����̊p�x��
            float Dot = dot(-g_DL_Dir, vNormal);
            // �}�C�i�X�l��0
            float DiffusePow = saturate(Dot);
            
            // ���K��Lambert
            DiffusePow /= 3.14159265358979f;
        
            // ���̐F * �ގ��̊g�U�F * �e
            outColor += (g_DL_Color * DiffusePow) * baseDiffuse * baseColor.a * shadow;
        }
    
        // Specular
        {
            // ���˂������̋���
            float spec = BlinnPhong(g_DL_Dir, vCam, vNormal, specPower);
            
            // ���̐F * ���ˌ��̋��� * �ގ��̔��ːF * �����l * �e
            outColor += (g_DL_Color * spec) * baseSpecular * baseColor.a * 0.5f * shadow;
        }
        
        float toBright = g_AL_Power;
        
        {
            //-------------------------
	        // �_��
	        //-------------------------
            for (int i = 0; i < g_PointLightNum.x; i++)
            {
                // �s�N�Z������_���ւ̕���
                float3 dir = g_PointLights[i].Pos - In.wPos;
                // �������Z�o
                float slLength = length(dir);
            
                // ���K��
                dir /= slLength;
            
                if (slLength < g_PointLights[i].Radius)
                {
                // �����ɂ�錸��
                    float atte = 1.0f - saturate(slLength / g_PointLights[i].Radius);
                
                    toBright += (1 - pow(1 - atte, 2)) * g_PointLights[i].IsBright;
                
                    atte *= atte;
                
                    // Diffuse
                    {
                        float Dot = dot(dir, vNormal);
                        float DiffusePow = saturate(Dot);
                    
                        DiffusePow *= atte;
                    
                        //DiffusePow /= 3.14159265358979f;
                
                        outColor += (g_PointLights[i].Color * DiffusePow) * baseDiffuse * baseColor.a;
                    }
            
                    // Specular
                    {
                        float spec = BlinnPhong(-dir, vCam, vNormal, specPower);
                        spec *= atte;
                        outColor += (g_PointLights[i].Color * spec) * baseSpecular * baseColor.a * 0.5f;
                    }
                }
            }
        }
        
        {
            //-------------------------
	        // �W����
	        //-------------------------
            for (int i = 0; i < g_SpotLightNum.x; i++)
            {
                if (g_SpotLights[i].IsEnable)
                {
                    float3 dir = g_SpotLights[i].Pos - In.wPos;
                
                    float slLength = length(dir);
                    
                    // ���K��
                    dir /= slLength;
                    
                    if (slLength < g_SpotLights[i].Range)
                    {
                        float rad = acos(dot(dir, -g_SpotLights[i].Dir));
                        float outerAngle = (g_SpotLights[i].Angle / 2.0);
                        
                        if (rad < outerAngle)
                        {
                            float angleIn = g_SpotLights[i].Angle * 0.8f;
                            float side = 1 - (rad - angleIn) / (g_SpotLights[i].Angle - angleIn);

                            float atte = saturate(1 - slLength / g_SpotLights[i].Range) * side;
                            
                            atte *= atte;
                            
                            // Diffuse
                            {
                                float lightDiffuse = saturate(dot(vNormal, dir));
                            
                                lightDiffuse *= atte;
                            
                                lightDiffuse /= 3.14159265358979f;
                            
                                outColor += (g_SpotLights[i].Color * lightDiffuse) * baseDiffuse * baseColor.a;
                            }
                            
                            // Specular
                            {
                                float spec = BlinnPhong(-dir, vCam, vNormal, specPower);
                                spec *= atte;
                                outColor += (g_SpotLights[i].Color * spec) * baseSpecular * baseColor.a * 0.5f;
                            }
                        }
                    }
                }
            }
        }
        
        // Emissive
        {
            outColor += g_emissiveTex.Sample(g_ssWL, In.UV).rgb * g_Emissive * In.Color.rgb;
        }
        
        // Ambient
        toBright = saturate(toBright);
        outColor += baseColor.rgb * toBright;
        
        // LimLight
        if(g_LimLightEnable)
        {
            float limPow = dot(normalize(In.wPos - g_CamPos), vNormal);
            limPow = 1 - abs(limPow);
            outColor.rgb += g_LimLightColor * pow(limPow, g_LimLightLevel);
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