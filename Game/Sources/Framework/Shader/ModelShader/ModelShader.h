#pragma once

#include "../ShaderBase.h"

class ModelShader : public ShaderBase
{
public:
	// ボーン最大数
	static const int maxBoneBufferSize = 300;

	bool Init()override;

	void Begin(int w = windowWidth,int h = windowHeight);

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
};