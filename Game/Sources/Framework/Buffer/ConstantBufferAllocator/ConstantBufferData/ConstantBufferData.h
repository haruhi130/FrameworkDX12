#pragma once

// シェーダーへ転送する定数バッファデータ群
namespace ConstantBufferData
{
	// カメラ情報
	struct CameraInfo
	{
		Math::Matrix mView;
		Math::Matrix mProj;
		Math::Matrix mProjInv;

		Math::Vector3 CamPos;
		float blank = 0.0f;
	};

	// オブジェクト個別情報
	struct ObjectInfo
	{
		// 描画するオブジェクトがスキンメッシュか判別
		int IsSkinMesh = 0;

		// アルファディザを行うか判別
		int IsDitherEnable = 0;

		// リムライト情報
		int				LimLightEnable = 0;
		float			LimLightLevel = 1;
		Math::Vector3	LimLightColor = { 1,1,1 };

		// パッキング規約
		float blank = 0.0f;
	};

	// ボーン情報
	struct SkinMeshInfo
	{
		Math::Matrix mBones[300];
	};

	// モデルマテリアル情報
	struct MaterialInfo
	{
		Math::Vector4 BaseColor;
		Math::Vector3 Emissive;
		float Metallic = 0.0f;
		float Roughness = 1.0f;

		// パッキング規約
		float blank[3] = { 0.0f,0.0f,0.0f };
	};

	// 2D描画情報
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

	// ライト情報
	struct LightInfo
	{
		// ライトに影響するか
		int IsUseLight = 0;

		// 平行光情報
		Math::Vector3 DirectionalLightDir = { -1,-1,-1 };
		Math::Vector3 DirectionalLightColor = { 1,1,1 };
		
		// 環境光の強さ
		float AmbientLightPower = 1.0f;

		// 影生成用ライトカメラ
		Math::Matrix DirLight_mVP;

		//----------------------------------------
		// 点光
		//----------------------------------------
		static const int MaxPointLightNum = 100;
		int PointLight_Num = 0;

		// パッキング規約
		float blank[3] = { 0.0f,0.0f,0.0f };

		std::array<PointLight,MaxPointLightNum> PointLights;
	
		//----------------------------------------
		// 集中光
		//----------------------------------------
		static const int MaxSpotLightNum = 100;
		int SpotLight_Num = 0;

		// パッキング規約
		float blank2[3] = { 0.0f,0.0f,0.0f };

		std::array<SpotLight, MaxSpotLightNum> SpotLights;
	};
}