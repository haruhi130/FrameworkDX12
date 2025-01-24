#pragma once
#include "../ShaderBase.h"

class ModelShader : public ShaderBase
{
public:
	// �{�[���ő吔
	static const int maxBoneBufferSize = 128;

	// ������
	bool Init() override;

	// �V�F�[�_�[�ݒ�
	void Begin(int w = window_width,int h = window_height);
	void BeginShadow(int w = shadow_definition,int h = shadow_definition);

	// ���b�V���`��
	void DrawMesh(const Mesh* mesh, const Math::Matrix& mWorld, const std::vector<Material>& materials);

	// ���f���`��
	void DrawModel(const ModelData& modelData, const Math::Matrix& mWorld = Math::Matrix::Identity,bool isUseLight = true);
	void DrawModel(ModelWork& modelWork, const Math::Matrix& mWorld = Math::Matrix::Identity, bool isUseLight = true);

	void SetDitherEnable(bool isEnable = true)
	{
		m_cbObject.IsDitherEnable = isEnable;
		m_dirtyCBObj = true;
	}

	void ResetCBObject()
	{
		m_cbObject = ConstantBufferData::ObjectInfo();
		GraphicsDevice::GetInstance().GetConstantBufferAllocator()
			->BindAndAttachData(1, m_cbObject);
		m_dirtyCBObj = false;
	}

private:
	// �X�L�����b�V���`��
	void DrawSkinMesh(ModelWork& modelWork);

	// �}�e���A���Z�b�g
	void SetMaterial(const Material& material);

	// �e�`��p�p�C�v���C��
	std::shared_ptr<Pipeline> m_spPipelineShadow = nullptr;

	// �e�`��p�[�x�e�N�X�`��
	std::shared_ptr<Texture> m_spShadowTex = nullptr;

	ConstantBufferData::ObjectInfo m_cbObject;

	bool m_dirtyCBObj = false;

	ConstantBufferData::SkinMeshInfo m_cbSkinMesh;

	// �}�e���A���萔�o�b�t�@
	ConstantBufferData::MaterialInfo m_cbMaterial;
};