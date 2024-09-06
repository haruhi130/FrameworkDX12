#pragma once

namespace ConstantBufferData
{
	// カメラ用構造体
	struct Camera
	{
		Math::Matrix mView;
		Math::Matrix mProj;
		Math::Matrix mProjInv;

		Math::Vector3 CamPos;
	};

	// スキンメッシュ判別
	struct Object
	{
		int isSkinMesh = 0;
	};

	// スキンメッシュ用ボーン構造体
	struct Bone
	{
		Math::Matrix mBones[300];
	};
}