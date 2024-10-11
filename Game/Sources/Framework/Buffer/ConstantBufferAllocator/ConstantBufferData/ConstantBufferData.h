#pragma once

namespace ConstantBufferData
{
	// �J�����p�\����
	struct Camera
	{
		Math::Matrix mView;
		Math::Matrix mProj;
		Math::Matrix mProjInv;

		Math::Vector3 CamPos;
	};

	// �X�L�����b�V������
	struct Object
	{
		int IsSkinMesh = 0;
	};

	// �X�L�����b�V���p�\����
	struct Bone
	{
		Math::Matrix mBones[300];
	};

	struct Material
	{
		Math::Vector4 BaseColor;
		Math::Vector3 Emissive;
		float Metallic = 0.0f;
		float Roughness = 1.0f;
	};

	// 2D�`��p�\����
	struct Sprite
	{
		Math::Matrix mTransform;
		Math::Vector4 Color = { 1,1,1,1 };
	};

	// �ˉe�s��P��
	struct Projection
	{
		Math::Matrix mProj;
	};

	// ���C�g
	struct Light
	{
		float AmbientLightPower = 1.0f;
		Math::Vector3 DirectionalLightDir = { 1,-1,1 };
		Math::Vector3 DirectionalLightColor = { 1,1,1 };
		int IsUseLight = 0;
	};
}