#pragma once

#include "../ShaderBase.h"

class ModelShader : public ShaderBase
{
public:
	// �{�[���ő吔
	static const int maxBoneBufferSize = 300;

	bool Init();

	void Begin();

	// ���b�V���`��
	void DrawMesh(const Mesh* mesh, const Math::Matrix& mWorld, const std::vector<Material>& materials);

	// ���f���`��
	void DrawModel(const ModelData& modelData, const Math::Matrix& mWorld = Math::Matrix::Identity);
	void DrawModel(ModelWork& modelWork, const Math::Matrix& mWorld = Math::Matrix::Identity);

private:
	void DrawSkinMesh(ModelWork& modelWork);
};