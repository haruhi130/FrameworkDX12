#pragma once

// �V�F�[�_�[�֓]������萔�o�b�t�@�f�[�^�Q
namespace ConstantBufferData
{
	// �J�������
	struct CameraInfo
	{
		Math::Matrix mView;
		Math::Matrix mProj;
		Math::Matrix mProjInv;

		Math::Vector3 CamPos;
		float blank = 0.0f;
	};

	// �I�u�W�F�N�g�ʏ��
	struct ObjectInfo
	{
		// �`�悷��I�u�W�F�N�g���X�L�����b�V��������
		int IsSkinMesh = 0;

		// �A���t�@�f�B�U���s��������
		int IsDitherEnable = 0;

		// �������C�g���
		int				LimLightEnable = 0;
		float			LimLightLevel = 1;
		Math::Vector3	LimLightColor = { 1,1,1 };

		// �p�b�L���O�K��
		float blank = 0.0f;
	};

	// �{�[�����
	struct SkinMeshInfo
	{
		Math::Matrix mBones[300];
	};

	// ���f���}�e���A�����
	struct MaterialInfo
	{
		Math::Vector4 BaseColor;
		Math::Vector3 Emissive;
		float Metallic = 0.0f;
		float Roughness = 1.0f;

		// �p�b�L���O�K��
		float blank[3] = { 0.0f,0.0f,0.0f };
	};

	// 2D�`����
	struct SpriteInfo
	{
		Math::Matrix mTransform;
		Math::Vector4 Color = { 1,1,1,1 };
	};

	// �ˉe�s��
	struct ProjectionInfo
	{
		Math::Matrix mProj;
	};

	// �|�C���g���C�g
	struct PointLight
	{
		PointLight() {}
		PointLight(const Math::Vector3& color, float radius, const Math::Vector3& pos, int isBright)
			:Color(color), Radius(radius), Pos(pos), IsBright(isBright) {}

		Math::Vector3 Color;	// ���̐F
		float Radius = 0.0f;	// ���͈̔�
		Math::Vector3 Pos;		// ���̈ʒu
		int IsBright = 0;		// ���x�p���C�g��
	};

	// �X�|�b�g���C�g
	struct SpotLight
	{
		SpotLight() {}
		SpotLight(int isEnable,const Math::Vector3& color,float range,const Math::Vector3& pos,float angle,const Math::Vector3& dir)
			:IsEnable(isEnable),Color(color),Range(range),Pos(pos),Angle(angle),Dir(dir) {}

		int IsEnable = 0;		// �L��
		Math::Vector3 Color;	// ���̐F
		float Range = 0.0f;		// ���͈̔�
		Math::Vector3 Pos;		// ���̈ʒu
		float Angle = 0.0f;		// ���ʊp�x
		Math::Vector3 Dir;		// ���̕���
	};

	// ���C�g���
	struct LightInfo
	{
		// ���C�g�ɉe�����邩
		int IsUseLight = 0;

		// ���s�����
		Math::Vector3 DirectionalLightDir = { -1,-1,-1 };
		Math::Vector3 DirectionalLightColor = { 1,1,1 };
		
		// �����̋���
		float AmbientLightPower = 1.0f;

		// �e�����p���C�g�J����
		Math::Matrix DirLight_mVP;

		//----------------------------------------
		// �_��
		//----------------------------------------
		static const int MaxPointLightNum = 100;
		int PointLight_Num = 0;

		// �p�b�L���O�K��
		float blank[3] = { 0.0f,0.0f,0.0f };

		std::array<PointLight,MaxPointLightNum> PointLights;
	
		//----------------------------------------
		// �W����
		//----------------------------------------
		static const int MaxSpotLightNum = 100;
		int SpotLight_Num = 0;

		// �p�b�L���O�K��
		float blank2[3] = { 0.0f,0.0f,0.0f };

		std::array<SpotLight, MaxSpotLightNum> SpotLights;
	};
}