#pragma once

// シェーダーへ転送する定数バッファデータ群
namespace ConstantBufferData
{
	// カメラ用構造体
	struct CameraInfo
	{
		Math::Matrix mView;
		Math::Matrix mProj;
		Math::Matrix mProjInv;

		Math::Vector3 CamPos;
	};

	// スキンメッシュ判別
	struct ObjectInfo
	{
		int IsSkinMesh = 0;
	};

	// スキンメッシュ用構造体
	struct BoneInfo
	{
		Math::Matrix mBones[300];
	};

	struct MaterialInfo
	{
		Math::Vector4 BaseColor;
		Math::Vector3 Emissive;
		float Metallic = 0.0f;
		float Roughness = 1.0f;
	};

	// 2D描画用構造体
	struct SpriteInfo
	{
		Math::Matrix mTransform;
		Math::Vector4 Color = { 1,1,1,1 };
	};

	// 射影行列単体
	struct ProjectionInfo
	{
		Math::Matrix mProj;
	};

	// ライト
	struct LightInfo
	{
		int IsUseLight = 0;
		Math::Vector3 DirectionalLightDir = { -1,-1,-1 };
		Math::Vector3 DirectionalLightColor = { 1,1,1 };
		float AmbientLightPower = 1.0f;
	};
}