#pragma once

#include "../ShaderBase.h"

class ModelShader : public ShaderBase
{
public:
	// �{�[���ő吔
	static const int maxBoneBufferSize = 300;

	bool Init()override;

	void Begin(int w = windowWidth,int h = windowHeight);

	// ���b�V���`��
	void DrawMesh(const Mesh* mesh, const Math::Matrix& mWorld, const std::vector<Material>& materials);

	// ���f���`��
	void DrawModel(const ModelData& modelData, const Math::Matrix& mWorld = Math::Matrix::Identity,bool isUseLight = true);
	void DrawModel(ModelWork& modelWork, const Math::Matrix& mWorld = Math::Matrix::Identity, bool isUseLight = true);

private:
	// �X�L�����b�V���`��
	void DrawSkinMesh(ModelWork& modelWork);

	// �}�e���A���Z�b�g
	void SetMaterial(const Material& material);

	// �}�e���A���萔�o�b�t�@
	ConstantBufferData::MaterialInfo m_cbMaterial;
};