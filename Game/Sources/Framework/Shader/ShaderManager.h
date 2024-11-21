#pragma once

#include "ModelShader/ModelShader.h"
#include "SpriteShader/SpriteShader.h"
#include "PostProcessShader/PostProcessShader.h"

class ShaderManager
{
public:
	// シェーダー初期化
	bool Init();

	//-------------------------------------------------
	// カメラ設定
	// カメラ情報を設定
	void SetCamera(const Math::Matrix& view,const Math::Matrix& proj);
	// 設定したカメラ情報をシェーダーへ転送
	void WriteCBCamera() const;

	const ConstantBufferData::CameraInfo& GetCBCamera() const
	{ return m_cbCamera; }

	//-------------------------------------------------
	// ライト設定
	// 陰影の有無を設定・転送
	void SetIsUseLight(bool isUseLight);
	// 平行光の方向と色を設定
	void SetDirectionalLight(const Math::Vector3& dir, const Math::Vector3& color);
	// 環境光の強さを設定
	void SetAmbientLight(float pow);

	void SetDirLightShadowArea(const Math::Vector2& area, float height);

	// 設定したライト情報をシェーダーへ転送
	void WriteCBLight() const;

	void WriteCBShadowArea(const Math::Matrix& proj, float dirLightHeight);
	
	void WriteLightParams();

	//-------------------------------------------------
	// シェーダー
	//-------------------------------------------------
	ModelShader m_modelShader;
	SpriteShader m_spriteShader;
	PostProcessShader m_postProcessShader;

private:
	// カメラ定数バッファ
	ConstantBufferData::CameraInfo m_cbCamera;

	// ライト定数バッファ
	ConstantBufferData::LightInfo m_cbLight;

	// 影描画範囲
	Math::Matrix m_shadowProj;
	// ライトの疑似高さ
	float m_dirLightHeight = 0.0f;

public:
	static ShaderManager& GetInstance()
	{
		static ShaderManager instance;
		return instance;
	}
};
