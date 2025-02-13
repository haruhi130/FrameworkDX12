#pragma once

#include "ModelShader/ModelShader.h"
#include "SpriteShader/SpriteShader.h"
#include "PostProcessShader/PostProcessShader.h"

class ShaderManager
{
public:
	// シェーダー初期化
	bool Init();

	void PreUpdate();

	//-------------------------------------------------
	// カメラ設定
	//-------------------------------------------------
	void SetCamera(const Math::Matrix& view,const Math::Matrix& proj);
	void WriteCBCamera() const;

	const ConstantBufferData::CameraInfo& GetCBCamera() const
	{ return m_cbCamera; }

	//-------------------------------------------------
	// ライト設定
	//-------------------------------------------------
	void SetIsUseLight(bool isUseLight);
	void SetDirectionalLight(const Math::Vector3& dir, const Math::Vector3& color = {1,1,1});
	void SetAmbientLight(float pow);
	void SetDirLightShadowArea(const Math::Vector2& area, float height);

	void WriteCBLight() const;
	void WriteCBShadowArea(const Math::Matrix& proj, float dirLightHeight);
	void WriteLightParams();
	void WritePointLight(const std::list<ConstantBufferData::PointLight>& pointLights);
	void WriteSpotLight(const std::list<ConstantBufferData::SpotLight>& spotLights);

	const ConstantBufferData::LightInfo& GetCBLight() const
	{ return m_cbLight; }

	//-------------------------------------------------
	// 光源追加
	//-------------------------------------------------
	// 点光追加
	void AddPointLight(const Math::Vector3& color, float radius, const Math::Vector3& pos, bool isBright);
	void AddPointLight(const ConstantBufferData::PointLight& pointLight);

	// 集中光追加
	void AddSpotLight(const Math::Vector3& color, float range, const Math::Vector3& pos, float angle, const Math::Vector3& dir,bool isEnable = true);
	void AddSpotLight(const ConstantBufferData::SpotLight& spotLight);

	//-------------------------------------------------
	// シェーダー
	//-------------------------------------------------
	ModelShader			m_modelShader;			// 3Dモデル描画シェーダー
	SpriteShader		m_spriteShader;			// 2Dテクスチャ描画シェーダー
	PostProcessShader	m_postProcessShader;	// ポストプロセス加工シェーダー

private:
	// カメラ定数バッファ
	ConstantBufferData::CameraInfo m_cbCamera;

	// ライト定数バッファ
	ConstantBufferData::LightInfo m_cbLight;

	// 影描画範囲
	Math::Matrix m_shadowProj;
	// 平行光源の高さ
	float m_dirLightHeight = 0.0f;

	// ポイントライトリスト
	std::list<ConstantBufferData::PointLight> m_pointLights;
	// スポットライトリスト
	std::list<ConstantBufferData::SpotLight> m_spotLights;

public:
	static ShaderManager& GetInstance()
	{
		static ShaderManager instance;
		return instance;
	}
};
