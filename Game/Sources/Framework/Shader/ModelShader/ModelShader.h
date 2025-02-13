#pragma once
#include "../ShaderBase.h"

class ModelShader : public ShaderBase
{
public:
	// ボーン最大数
	static const int maxBoneBufferSize = 300;

	// 初期化
	bool Init() override;

	// シェーダー設定
	void Begin(int w = window_width,int h = window_height);
	void BeginShadow(int w = shadow_definition,int h = shadow_definition);

	// メッシュ描画
	void DrawMesh(const Mesh* mesh, const Math::Matrix& mWorld, const std::vector<Material>& materials);

	// モデル描画
	void DrawModel(const ModelData& modelData, const Math::Matrix& mWorld = Math::Matrix::Identity,bool isUseLight = true);
	void DrawModel(ModelWork& modelWork, const Math::Matrix& mWorld = Math::Matrix::Identity, bool isUseLight = true);

	// アルファディザ設定
	void SetDitherEnable(bool isEnable = true)
	{
		m_cbObject.IsDitherEnable = isEnable;
		m_dirtyCBObj = true;
	}

	// リムライト設定
	void SetLimLightEnable(bool isEnable)
	{
		m_cbObject.LimLightEnable = isEnable;
		m_dirtyCBObj = true;
	}

	// リムライトの強さ・色設定
	void SetLimLight(float level = 1.0f,const Math::Vector3& color = {1.0f,1.0f,1.0f})
	{
		m_cbObject.LimLightLevel = level;
		m_cbObject.LimLightColor = color;

		m_dirtyCBObj = true;
	}

	// 定数バッファを初期状態にリセット
	void ResetCBObject()
	{
		m_cbObject = ConstantBufferData::ObjectInfo();
		GraphicsDevice::GetInstance().GetConstantBufferAllocator()
			->BindAndAttachData(1, m_cbObject);
		m_dirtyCBObj = false;
	}

private:
	// スキンメッシュ描画
	void DrawSkinMesh(ModelWork& modelWork);

	// マテリアルセット
	void SetMaterial(const Material& material);

	// 影描画用パイプライン
	std::shared_ptr<Pipeline> m_spPipelineShadow = nullptr;

	// 影描画用深度テクスチャ
	std::shared_ptr<Texture> m_spShadowTex = nullptr;

	ConstantBufferData::ObjectInfo m_cbObject;

	bool m_dirtyCBObj = false;

	ConstantBufferData::SkinMeshInfo m_cbSkinMesh;

	// マテリアル定数バッファ
	ConstantBufferData::MaterialInfo m_cbMaterial;
};