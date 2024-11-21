//=====================================================
//
// �V�F�[�_�[�ɋ��ʂœn�����
//
//=====================================================

// �J����
cbuffer cbCamera : register(b4)
{
    row_major float4x4 g_mView;     // �r���[�ϊ��s��
    row_major float4x4 g_mProj;     // �ˉe�ϊ��s��
    row_major float4x4 g_mProjInv;  // �ˉe�ϊ��t�s��
    float3             g_camPos;    // �J�������W
}

// ���C�g
cbuffer cbLight : register(b5)
{
    // ���C�g�̎g�p����
    int g_isUseLight;
    
    // ���s��
    float3  g_DL_Dir;    // ���̕���
    float3  g_DL_Color;  // ���̐F
    
    // ����
    float g_AL_Power;
    
    row_major float4x4 g_DL_mVP;
}