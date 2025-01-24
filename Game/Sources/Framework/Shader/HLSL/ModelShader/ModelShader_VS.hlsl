#include "inc_ModelShader.hlsli"
#include "../inc_Common.hlsli"

VSOutput main( 
float4 pos : POSITION,float2 uv : TEXCOORD,float3 normal : NORMAL,
float4 color : COLOR,float3 tangent :TANGENT,uint4 skinIndex : SKININDEX,float4 skinWeight : SKINWEIGHT)
{
    // �X�L�����b�V������
    if (g_isSkinMeshObj)
    {
        row_major float4x4 mBones = 0;
        [unroll]
        for (int i = 0; i < 4; i++)
        {
            mBones += g_mBones[skinIndex[i]] * skinWeight[i];
        }
    
        pos = mul(pos, mBones);
        normal = mul(normal, (float3x3) mBones);
    }
    
    VSOutput Out;
    
    // ���W�ϊ�
    Out.pos = mul(pos, g_mWorld);
    Out.wPos = Out.pos.xyz;
    Out.svPos = mul(Out.pos, g_mView);
    Out.wvPos = Out.svPos.xyz;
    Out.svPos = mul(Out.svPos, g_mProj);
    Out.tPos = mul(Out.pos,g_DL_mVP);
    
    // UV���W
    Out.uv = uv;
    
    Out.tangent = tangent;
    
    // ���_�F
    Out.color = color;
    
    // �@��
    Out.wN = normalize(mul(normal, (float3x3) g_mWorld));
    Out.wT = normalize(mul(tangent, (float3x3) g_mWorld));
    float3 binormal = cross(normal, tangent);
    Out.wB = normalize(mul(binormal, (float3x3)g_mWorld));
    
    return Out;
}

float4 Shadow(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD
, uint4 skinIndex : SKININDEX, float4 skinWeight : SKINWEIGHT) : SV_Position
{
    // �X�L�����b�V������
    if (g_isSkinMeshObj)
    {
        row_major float4x4 mBones = 0;
        [unroll]
        for (int i = 0; i < 4; i++)
        {
            mBones += g_mBones[skinIndex[i]] * skinWeight[i];
        }
    
        pos = mul(pos, mBones);
        normal = mul(normal, (float3x3) mBones);
    }
    
    pos = mul(pos, g_mWorld);
    
    return mul(pos, g_DL_mVP);
}