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

	struct ObjectInfo
	{
		int IsSkinMesh = 0;

		int IsDitherEnable = 0;

		int blank[2] = { 0,0 };
	};

	// スキンメッシュ用構造体
	struct SkinMeshInfo
	{
		Math::Matrix mBones[128];
	};

	// モデルマテリアル情報
	struct MaterialInfo
	{
		Math::Vector4 BaseColor;
		Math::Vector3 Emissive;
		float Metallic = 0.0f;
		float Roughness = 1.0f;

		float blank[3] = { 0,0,0 };
	};

	// 2D描画用構造体
	struct SpriteInfo
	{
		Math::Matrix mTransform;
		Math::Vector4 Color = { 1,1,1,1 };
	};

	// 射影行列
	struct ProjectionInfo
	{
		Math::Matrix mProj;
	};

	// ポイントライト
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

	// ライト
	struct LightInfo
	{
		float IsUseLight = 0.0f;
		Math::Vector3 DirectionalLightDir = { -1,-1,-1 };
		Math::Vector3 DirectionalLightColor = { 1,1,1 };
		float AmbientLightPower = 1.0f;

		// ライトカメラ
		Math::Matrix DirLight_mVP;

		static const int MaxPointLightNum = 100;

		int PointLightNum = 0;
		int blank[2] = { 0,0 };

		std::array<PointLight, MaxPointLightNum> PointLights;
	};
}