#pragma once

// �V�F�[�_�[�֓]������萔�o�b�t�@�f�[�^�Q
namespace ConstantBufferData
{
	// �J�����p�\����
	struct CameraInfo
	{
		Math::Matrix mView;
		Math::Matrix mProj;
		Math::Matrix mProjInv;

		Math::Vector3 CamPos;
		float blank = 0.0f;
	};

	struct ObjectInfo
	{
		int IsSkinMesh = 0;

		int IsDitherEnable = 0;

		float blank[2] = { 0.0f,0.0f };
	};

	// �X�L�����b�V���p�\����
	struct SkinMeshInfo
	{
		Math::Matrix mBones[128];
	};

	// ���f���}�e���A�����
	struct MaterialInfo
	{
		Math::Vector4 BaseColor;
		Math::Vector3 Emissive;
		float Metallic = 0.0f;
		float Roughness = 1.0f;

		float blank[3] = { 0.0f,0.0f,0.0f };
	};

	// 2D�`��p�\����
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

	// ���C�g
	struct LightInfo
	{
		int IsUseLight = 0;
		Math::Vector3 DirectionalLightDir = { -1,-1,-1 };
		Math::Vector3 DirectionalLightColor = { 1,1,1 };
		float AmbientLightPower = 1.0f;

		// ���C�g�J����
		Math::Matrix DirLight_mVP;

		static const int MaxPointLightNum = 100;
		int PointLight_Num = 0;
		float blank[3] = { 0.0f,0.0f,0.0f };
		std::array<PointLight,MaxPointLightNum> PointLights;
	
		static const int MaxSpotLightNum = 100;
		int SpotLight_Num = 0;
		float blank2[3] = { 0.0f,0.0f,0.0f };
		std::array<SpotLight, MaxSpotLightNum> SpotLights;
	};
}