#include "ModelShader.h"

bool ModelShader::Init()
{
	ShaderBase::Init();

	m_spShadowTex = std::make_shared<Texture>();
	m_spShadowTex->CreateLightDepthSRV();

	// �V�F�[�_�[�ɓn�����ݒ�
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL,InputLayout::COLOR,InputLayout::TANGENT,InputLayout::SKININDEX,InputLayout::SKINWEIGHT };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	Create(m_spRootSignature, m_spPipeline, L"ModelShader/ModelShader", renderingSetting,
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,
		RangeType::SRV,RangeType::SRV,RangeType::SRV ,RangeType::SRV ,RangeType::SRV });

	// �V���h�E�}�b�v�p�Ƀp�C�v���C����ݒ�
	//LoadShaderFile(L"ModelShader/ModelShader", "Shadow");
	ID3DBlob* pErrorBlob = nullptr;
	auto result = D3DCompileFromFile(L"Sources/Framework/Shader/HLSL/ModelShader/ModelShader_VS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "Shadow",
		"vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, m_cpVSBlob.GetAddressOf(), &pErrorBlob);
	if (FAILED(result))
	{
		assert(0 && "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂���");
		return false;
	}
	
	renderingSetting.CullMode = CullMode::Front;
	renderingSetting.RTVCount = 0;
	renderingSetting.Formats = { DXGI_FORMAT_UNKNOWN };
	m_spPipelineShadow = std::make_shared<Pipeline>();
	m_spPipelineShadow->SetRenderSettings(m_spRootSignature.get(), renderingSetting.InputLayouts,
		renderingSetting.CullMode, renderingSetting.BlendMode, renderingSetting.PrimitiveTopologyType);
	m_spPipelineShadow->Create({ m_cpVSBlob.Get(),m_cpHSBlob.Get() ,m_cpDSBlob.Get() ,m_cpGSBlob.Get(),nullptr }, renderingSetting.Formats,
		renderingSetting.IsDepth, renderingSetting.IsDepthMask, renderingSetting.RTVCount, renderingSetting.IsWireFrame);

	return true;
}

void ModelShader::Begin(int w, int h)
{
	ShaderBase::Begin(m_spRootSignature, m_spPipeline, w, h);

	// �J�������
	ShaderManager::GetInstance().WriteCBCamera();

	// ���C�g���
	ShaderManager::GetInstance().WriteCBLight();

	m_spShadowTex->Set(m_cbvCount + 4);
}

void ModelShader::BeginShadow()
{
	// �V���h�E�}�b�v�p�̃n���h���擾
	auto handle = GraphicsDevice::GetInstance().GetDSVHeap()->GetCPUHandle(
		GraphicsDevice::GetInstance().GetLightDepthStencil()->GetDSVNumber());

	// �����_�[�^�[�Q�b�g�ύX(Shadow��PixelShader���g�p���Ȃ��̂�rtvH��nullptr)
	GraphicsDevice::GetInstance().GetCmdList()->OMSetRenderTargets(0, nullptr, false, &handle);

	// �V���h�E�}�b�v�p�̐[�x�o�b�t�@�N���A
	GraphicsDevice::GetInstance().GetLightDepthStencil()->ClearBuffer();

	// ���[�g�V�O�l�`���ƃp�C�v���C���ݒ�
	ShaderBase::Begin(m_spRootSignature, m_spPipelineShadow, 1024, 1024);

	// ���C�g���̐ݒ�
	ShaderManager::GetInstance().WriteLightParams();
}

void ModelShader::DrawMesh(const Mesh* mesh, const Math::Matrix& mWorld, const std::vector<Material>& materials)
{
	if (mesh == nullptr) { return; }

	// �V�F�[�_�[�֍s���ݒ�
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(0, mWorld);

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
			->BindAndAttachData(1, obj);

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
		->BindAndAttachData(2, bone);
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
		->BindAndAttachData(3, m_cbMaterial);

	// �}�e���A���摜��ݒ�
	if (material.spBaseColorTex != nullptr)
	{
		material.spBaseColorTex->Set(m_cbvCount);
	}
	if (material.spEmissiveTex != nullptr)
	{
		material.spEmissiveTex->Set(m_cbvCount + 1);
	}
	if (material.spMetallicRoughnessTex != nullptr)
	{
		material.spMetallicRoughnessTex->Set(m_cbvCount + 2);
	}
	if (material.spNormalTex != nullptr)
	{
		material.spNormalTex->Set(m_cbvCount + 3);
	}
}
