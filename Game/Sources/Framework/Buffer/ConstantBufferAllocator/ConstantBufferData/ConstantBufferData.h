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
		float blank = 0.0f;
	};

	struct ObjectInfo
	{
		int IsSkinMesh = 0;

		int IsDitherEnable = 0;

		float blank[2] = { 0.0f,0.0f };
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

		float blank[3] = { 0.0f,0.0f,0.0f };
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

		Math::Vector3 Color;	// 光の色
		float Radius = 0.0f;	// 光の範囲
		Math::Vector3 Pos;		// 光の位置
		int IsBright = 0;		// 明度用ライトか
	};

	// スポットライト
	struct SpotLight
	{
		SpotLight() {}
		SpotLight(int isEnable,const Math::Vector3& color,float range,const Math::Vector3& pos,float angle,const Math::Vector3& dir)
			:IsEnable(isEnable),Color(color),Range(range),Pos(pos),Angle(angle),Dir(dir) {}

		int IsEnable = 0;		// 有効
		Math::Vector3 Color;	// 光の色
		float Range = 0.0f;		// 光の範囲
		Math::Vector3 Pos;		// 光の位置
		float Angle = 0.0f;		// 効果角度
		Math::Vector3 Dir;		// 光の方向
	};

	// ライト
	struct LightInfo
	{
		int IsUseLight = 0;
		Math::Vector3 DirectionalLightDir = { -1,-1,-1 };
		Math::Vector3 DirectionalLightColor = { 1,1,1 };
		float AmbientLightPower = 1.0f;

		// ライトカメラ
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