// �o�̓f�[�^
struct VSOutput
{
    float4 svPos    : SV_Position;  // �ˉe���W
    float3 wPos     : TEXCOORD0;    // ���[���h3D���W
    float2 UV       : TEXCOORD1;    // UV���W
    float3 Tangent  : TANGENT;      // �^���W�F���g�l
    float4 Color    : COLOR;        // �F
    
    float3 wN       : TEXCOORD2;    // ���[���h�@��
    float3 wT       : TEXCOORD3;    // ���[���h�ڐ�
    float3 wB       : TEXCOORD4;    // ���[���h�]�@��
    
    float4 Pos      : Position;     // �W�����W
    float3 wvPos    : TEXCOORD5;    
};

// ���[���h�ϊ��s��
cbuffer cbWorld : register(b0)
{
    row_major float4x4 g_mWorld;
}

// �I�u�W�F�N�g���
cbuffer cbObject : register(b1)
{
    int g_IsSkinMeshObj;    // �X�L�����b�V������
    
    int g_IsDitherEnable;   // �A���t�@�f�B�U�L��
    
    int g_LimLightEnable;   // �������C�g�L��
    float g_LimLightLevel;  // �������C�g�̏搔(����)
    float3 g_LimLightColor; // �������C�g�̐F
}

// �{�[�����
cbuffer cbBones : register(b2)
{
    row_major float4x4 g_mBones[300];
}

// �}�e���A���F
cbuffer cbMaterial : register(b3)
{
    float4 g_BaseCol;   // �x�[�X�F
    float3 g_Emissive;  // ���Ȕ����F
    float g_Metallic;   // �����x
    float g_Roughness;  // �e��
}