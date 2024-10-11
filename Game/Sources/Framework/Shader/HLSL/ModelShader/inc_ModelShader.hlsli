// �o�̓f�[�^
struct VSOutput
{
    float4 pos      : SV_Position;  // �ˉe���W
    float3 wPos     : TEXCOORD0;    // ���[���h3D���W
    float2 uv       : TEXCOORD1;    // UV���W
    float3 tangent  : TANGENT;      // �^���W�F���g�l
    float4 color    : COLOR;        // �F
    
    float3 wN       : TEXCOORD2;    // ���[���h�@��
    float3 wT       : TEXCOORD3;    // ���[���h�ڐ�
    float3 wB       : TEXCOORD4;    // ���[���h�]�@��
};

// ���[���h�ϊ��s��
cbuffer cbWorld : register(b1)
{
    row_major float4x4 g_mWorld;
}

// �X�L�����b�V������
cbuffer cbObject : register(b2)
{
    int g_isSkinMeshObj;
}

// �X�L�����b�V���{�[��
cbuffer cbBones : register(b3)
{
    row_major float4x4 g_mBones[300];
}

// �}�e���A���F
cbuffer cbMaterial : register(b4)
{
    float4 g_baseCol;   // �x�[�X�F
    float3 g_emissive;  // ���Ȕ����F
    float g_metallic;   // �����x
    float g_roughness;  // �e��
}