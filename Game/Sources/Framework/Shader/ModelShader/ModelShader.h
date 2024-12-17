#pragma once
#include "../ShaderBase.h"

class ModelShader : public ShaderBase
{
public:
	// ボーン最大数
	static const int maxBoneBufferSize = 128;

	// 初期化
	bool Init() override;

	// シェーダー設定
	void Begin(int w = window_width,int h = window_height);
	void BeginShadow(int w = shadow_difinition,int h = shadow_difinition);

	// メッシュ描画
	void DrawMesh(const Mesh* mesh, const Math::Matrix& mWorld, const std::vector<Material>& materials);

	// モデル描画
	void DrawModel(const ModelData& modelData, const Math::Matrix& mWorld = Math::Matrix::Identity,bool isUseLight = true);
	void DrawModel(ModelWork& modelWork, const Math::Matrix& mWorld = Math::Matrix::Identity, bool isUseLight = true);

private:
	// スキンメッシュ描画
	void DrawSkinMesh(ModelWork& modelWork);

	// マテリアルセット
	void SetMaterial(const Material& material);

	// マテリアル定数バッファ
	ConstantBufferData::MaterialInfo m_cbMaterial;

	// 影描画用パイプライン
	std::shared_ptr<Pipeline> m_spPipelineShadow = nullptr;

	// 影描画用深度テクスチャ
	std::shared_ptr<Texture> m_spShadowTex = nullptr;
};