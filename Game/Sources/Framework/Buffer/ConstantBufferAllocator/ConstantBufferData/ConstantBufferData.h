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
		int IsSkinMesh = 0;
	};

	// スキンメッシュ用構造体
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

	// 2D描画用構造体
	struct Sprite
	{
		Math::Matrix mTransform;
		Math::Vector4 Color = { 1,1,1,1 };
	};

	// 射影行列単体
	struct Projection
	{
		Math::Matrix mProj;
	};

	// ライト
	struct Light
	{
		float AmbientLightPower = 1.0f;
		Math::Vector3 DirectionalLightDir = { 1,-1,1 };
		Math::Vector3 DirectionalLightColor = { 1,1,1 };
		int IsUseLight = 0;
	};
}