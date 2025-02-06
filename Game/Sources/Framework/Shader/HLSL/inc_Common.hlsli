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
    float3             g_CamPos;    // �J�������W
}

struct PointLightInfo
{
    float3 Color;
    float Radius;
    float3 Pos;
    float IsBright;
};

struct SpotLightInfo
{
    int IsEnable;
    float3 Color;
    float Range;
    float3 Pos;
    float Angle;
    float3 Dir;
};

// ���C�g
cbuffer cbLight : register(b5)
{
    // ���C�g�̎g�p����
    int g_IsUseLight;
    
    // ���s��
    float3  g_DL_Dir;    // ���̕���
    float3  g_DL_Color;  // ���̐F
    
    // ����
    float g_AL_Power;
    
    row_major float4x4 g_DL_mVP;
    
    //-------------------------------
    // �_��
    //-------------------------------
    int4 g_PointLightNum;
    
    PointLightInfo g_PointLights[100];
    
    //-------------------------------
    // �W����
    //-------------------------------
    int4 g_SpotLightNum;
    
    SpotLightInfo g_SpotLights[100];
}