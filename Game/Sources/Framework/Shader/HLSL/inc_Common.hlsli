//=====================================================
//
// �S�V�F�[�_�[����
//
//=====================================================

// �J����
cbuffer cbCamera : register(b0)
{
    row_major float4x4 g_mView;
    row_major float4x4 g_mProj;
    row_major float4x4 g_mProjInv;
    float3 g_camPos;
}

// ���C�g
cbuffer cbLight : register(b5)
{
    // ����
    float g_AL_Power;
    
    // ���s��
    float3 g_DL_Dir;
    float3 g_DL_Color;
    
    int g_isUseLight;
}