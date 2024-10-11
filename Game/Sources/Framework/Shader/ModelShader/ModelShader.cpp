#include "ModelShader.h"

bool ModelShader::Init()
{
	// �V�F�[�_�[�ɓn�����ݒ�
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL,InputLayout::COLOR,InputLayout::TANGENT,InputLayout::SKININDEX,InputLayout::SKINWEIGHT };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	Create(L"ModelShader/ModelShader", renderingSetting,
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,
		RangeType::SRV,RangeType::SRV,RangeType::SRV ,RangeType::SRV });

	return true;
}

void ModelShader::Begin(int w, int h)
{
	ShaderBase::Begin(w,h);

	// �J�������
	ShaderManager::GetInstance().WriteCBCamera();

	// ���C�g���
	ShaderManager::GetInstance().WriteCBLight();
}

void ModelShader::DrawMesh(const Mesh* mesh, const Math::Matrix& mWorld, const std::vector<Material>& materials)
{
	if (mesh == nullptr) { return; }

	// �V�F�[�_�[�֍s���ݒ�
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(1, mWorld);

	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���Z�b�g
	mesh->SetToDevice();

	// �T�u�Z�b�g�`��
	for (UINT subi = 0; subi < mesh->GetSubsets().size(); subi++)
	{
		if (mesh->GetSubsets()[subi].FaceCount == 0)continue;

		// �}�e���A���擾
		const Material& material = materials[mesh->GetSubsets()[subi].MaterialNo];

		// �}�e���A���Z�b�g
		SetMaterial(material);

		mesh->DrawSubset(subi);
	}
}

void ModelShader::DrawModel(const ModelData& modelData, const Math::Matrix& mWorld, bool isUseLight)
{
	// ���C�g���g�p���邩
	ShaderManager::GetInstance().SetIsUseLight(isUseLight);

	// �m�[�h�擾
	auto& nodes = modelData.GetNodes();

	// ���b�V���`��
	for (auto& nodeIdx : modelData.GetDrawMeshNodeIndices())
	{
		DrawMesh(nodes[nodeIdx].spMesh.get(), nodes[nodeIdx].mWorld * mWorld, modelData.GetMaterials());
	}
}

void ModelShader::DrawModel(ModelWork& modelWork, const Math::Matrix& mWorld, bool isUseLight)
{
	const std::shared_ptr<ModelData>& data = modelWork.GetModelData();

	if (data == nullptr) { return; }

	// ���C�g���g�p���邩
	ShaderManager::GetInstance().SetIsUseLight(isUseLight);

	// �ċA����
	if (modelWork.IsNeedCalcNodeMatrices())
	{
		modelWork.CalcNodeMatrices();
	}

	// �X�L�����b�V��������
	{
		ConstantBufferData::ObjectInfo obj;

		obj.IsSkinMesh = data->IsSkinMesh();
		GraphicsDevice::GetInstance().GetConstantBufferAllocator()
			->BindAndAttachData(2, obj);

		// �X�L�����b�V���`��
		if (data->IsSkinMesh())
		{
			DrawSkinMesh(modelWork);
		}
	}

	// �m�[�h�擾
	auto& workNodes = modelWork.GetNodes();
	auto& dataNodes = data->GetNodes();

	// ���b�V���`��
	for (auto&& nodeIdx : data->GetDrawMeshNodeIndices())
	{
		DrawMesh(dataNodes[nodeIdx].spMesh.get(), workNodes[nodeIdx].mWorld * mWorld, data->GetMaterials());
	}
}

void ModelShader::DrawSkinMesh(ModelWork& modelWork)
{
	const std::shared_ptr<ModelData>& data = modelWork.GetModelData();

	auto& workNodes = modelWork.GetNodes();
	auto& dataNodes = data->GetNodes();

	ConstantBufferData::BoneInfo bone;
	for (auto&& nodeIdx : data->GetBoneNodeIndices())
	{
		if (nodeIdx >= maxBoneBufferSize)
		{
			assert(0 && "�]���ł���{�[����������I�[�o�[");
			return;
		}

		auto& dataNode = dataNodes[nodeIdx];
		auto& workNode = workNodes[nodeIdx];

		bone.mBones[dataNode.BoneIdx] = dataNode.mBoneInverseWorld * workNode.mWorld;
	}
	// �V�F�[�_�[�֌v�Z�����{�[����ݒ�
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(3, bone);
}

void ModelShader::SetMaterial(const Material& material)
{
	// �}�e���A���F��ݒ�
	m_cbMaterial.BaseColor = material.BaseColor;
	m_cbMaterial.Emissive = material.Emissive;
	m_cbMaterial.Metallic = material.Metallic;
	m_cbMaterial.Roughness = material.Roughness;

	// �}�e���A���F���V�F�[�_�[�֓]��
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(4, m_cbMaterial);

	// �}�e���A���摜��ݒ�
	material.spBaseColorTex->Set(m_cbvCount);
	material.spNormalTex->Set(m_cbvCount + 1);
	material.spMetallicRoughnessTex->Set(m_cbvCount + 2);
	material.spEmissiveTex->Set(m_cbvCount + 3);
}
