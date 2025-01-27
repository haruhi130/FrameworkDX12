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
	};

	struct ObjectInfo
	{
		int IsSkinMesh = 0;

		int IsDitherEnable = 0;

		int blank[2] = { 0,0 };
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

		float blank[3] = { 0,0,0 };
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

		Math::Vector3 Color;
		float Radius = 0.0f;
		Math::Vector3 Pos;
		int IsBright = 0;
	};

	// ���C�g
	struct LightInfo
	{
		float IsUseLight = 0.0f;
		Math::Vector3 DirectionalLightDir = { -1,-1,-1 };
		Math::Vector3 DirectionalLightColor = { 1,1,1 };
		float AmbientLightPower = 1.0f;

		// ���C�g�J����
		Math::Matrix DirLight_mVP;

		static const int MaxPointLightNum = 100;

		int PointLightNum = 0;
		int blank[2] = { 0,0 };

		std::array<PointLight, MaxPointLightNum> PointLights;
	};
}