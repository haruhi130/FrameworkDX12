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
		int isSkinMesh = 0;
	};

	// �X�L�����b�V���p�{�[���\����
	struct Bone
	{
		Math::Matrix mBones[300];
	};
}