#pragma once

#include "../ShaderBase.h"

class ModelShader : public ShaderBase
{
public:
	// ボーン最大数
	static const int maxBoneBufferSize = 300;

	bool Init();

	void Begin();

	// メッシュ描画
	void DrawMesh(const Mesh* mesh, const Math::Matrix& mWorld, const std::vector<Material>& materials);

	// モデル描画
	void DrawModel(const ModelData& modelData, const Math::Matrix& mWorld = Math::Matrix::Identity);
	void DrawModel(ModelWork& modelWork, const Math::Matrix& mWorld = Math::Matrix::Identity);

private:
	void DrawSkinMesh(ModelWork& modelWork);
};