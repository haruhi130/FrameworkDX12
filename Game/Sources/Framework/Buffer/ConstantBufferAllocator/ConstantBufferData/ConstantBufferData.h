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

	// �X�L�����b�V������
	struct ObjectInfo
	{
		int IsSkinMesh = 0;
	};

	// �X�L�����b�V���p�\����
	struct BoneInfo
	{
		Math::Matrix mBones[256];
	};

	// ���f���}�e���A�����
	struct MaterialInfo
	{
		Math::Vector4 BaseColor;
		Math::Vector3 Emissive;
		float Metallic = 0.0f;
		float Roughness = 1.0f;
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

	// ���C�g
	struct LightInfo
	{
		float IsUseLight = 0.0f;
		Math::Vector3 DirectionalLightDir = { -0.5f,-1,-1 };
		Math::Vector3 DirectionalLightColor = { 1,1,1 };
		float AmbientLightPower = 1.0f;

		// ���C�g�J����
		Math::Matrix DirLight_mVP;
	};
}