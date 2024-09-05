#include "Shader.h"

void Shader::Create(const std::wstring& filePath,
	const RenderingSetting& renderingSetting, const std::vector<RangeType>& rangeTypes)
{
	LoadShaderFile(filePath);

	m_upRootSignature = std::make_unique<RootSignature>();
	m_upRootSignature->Create(rangeTypes, m_cbvCount);

	m_upPipeline = std::make_unique<Pipeline>();
	m_upPipeline->SetRenderSettings(m_upRootSignature.get(), renderingSetting.InputLayouts,
		renderingSetting.CullMode, renderingSetting.BlendMode, renderingSetting.PrimitiveTopologyType);
	m_upPipeline->Create({ m_pVSBlob.Get() ,m_pHSBlob.Get() ,m_pDSBlob.Get() ,m_pGSBlob.Get() ,m_pPSBlob.Get() }, renderingSetting.Formats,
		renderingSetting.IsDepth, renderingSetting.IsDepthMask, renderingSetting.RTVCount, renderingSetting.IsWireFrame);
}

void Shader::Begin(int w, int h)
{
	// �p�C�v���C���Z�b�g
	GraphicsDevice::GetInstance().GetCmdList()->SetPipelineState(m_upPipeline->GetPipeline());

	// ���[�g�V�O�l�`���Z�b�g
	GraphicsDevice::GetInstance().GetCmdList()->SetGraphicsRootSignature(m_upRootSignature->GetRootSignature());

	// �g�|���W�[�ݒ�
	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType =
		static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(m_upPipeline->GetTopologyType());

	switch (topologyType)
	{
	case D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT:
		GraphicsDevice::GetInstance().GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		break;
	case D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE:
		GraphicsDevice::GetInstance().GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	case D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE:
		GraphicsDevice::GetInstance().GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	case D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH:
		GraphicsDevice::GetInstance().GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
		break;
	}

	// �r���[�|�[�g�ݒ�
	D3D12_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(w);
	viewport.Height = static_cast<float>(h);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	GraphicsDevice::GetInstance().GetCmdList()->RSSetViewports(1, &viewport);

	// ���N�g�ݒ�
	D3D12_RECT rect = {};
	rect.right = w;
	rect.bottom = h;
	GraphicsDevice::GetInstance().GetCmdList()->RSSetScissorRects(1, &rect);
}

void Shader::DrawMesh(const Mesh* mesh, const Math::Matrix& mWorld, const std::vector<Material>& materials)
{
	if (mesh == nullptr) { return; }

	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(1,mWorld);

	mesh->SetToDevice();

	// �T�u�Z�b�g
	for (UINT subi = 0; subi < mesh->GetSubsets().size(); subi++)
	{
		if (mesh->GetSubsets()[subi].FaceCount == 0)continue;

		const Material& material = materials[mesh->GetSubsets()[subi].MaterialNo];

		SetMaterial(material);

		mesh->DrawSubset(subi);
	}
}

void Shader::DrawModel(const ModelData& modelData, const Math::Matrix& mWorld)
{
	auto& nodes = modelData.GetNodes();

	for (auto& nodeIdx : modelData.GetDrawMeshNodeIndices())
	{
		DrawMesh(nodes[nodeIdx].spMesh.get(), nodes[nodeIdx].mLocal * mWorld, modelData.GetMaterials());
	}
}

void Shader::DrawModel(ModelWork& modelWork, const Math::Matrix& mWorld)
{
	const std::shared_ptr<ModelData>& data = modelWork.GetModelData();

	if (data == nullptr) { return; }

	if (modelWork.IsNeedCalcNodeMatrices())
	{
		modelWork.CalcNodeMatrices();
	}

	auto& workNodes = modelWork.GetNodes();
	auto& dataNodes = data->GetNodes();

	for (auto& nodeIdx : data->GetDrawMeshNodeIndices())
	{
		DrawMesh(dataNodes[nodeIdx].spMesh.get(), workNodes[nodeIdx].mLocal * mWorld, data->GetMaterials());
	}
}

void Shader::LoadShaderFile(const std::wstring& filePath)
{
	ID3DInclude* include = D3D_COMPILE_STANDARD_FILE_INCLUDE;
	UINT flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ID3DBlob* pErrorBlob = nullptr;

	std::wstring format = L".hlsl";
	std::wstring currentPath = L"Sources/Framework/Shader/HLSL/";

	// ���_�V�F�[�_�[�̃R���p�C��
	{
		std::wstring fullFilepath = currentPath + filePath + L"_VS" + format;
		auto result = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, "main",
			"vs_5_0", flag, 0, m_pVSBlob.GetAddressOf(), &pErrorBlob);

		if (FAILED(result))
		{
			assert(0 && "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂���");
			return;
		}
	}

	// �n���V�F�[�_�[�̃R���p�C��
	{
		std::wstring fullFilepath = currentPath + filePath + L"_HS" + format;
		auto result = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, "main",
			"hs_5_0", flag, 0, m_pHSBlob.GetAddressOf(), &pErrorBlob);
	}

	// �h���C���V�F�[�_�[�̃R���p�C��
	{
		std::wstring fullFilepath = currentPath + filePath + L"_DS" + format;
		auto result = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, "main",
			"ds_5_0", flag, 0, m_pDSBlob.GetAddressOf(), &pErrorBlob);
	}

	// �W�I���g���V�F�[�_�[�̃R���p�C��
	{
		std::wstring fullFilepath = currentPath + filePath + L"_GS" + format;
		auto result = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, "main",
			"gs_5_0", flag, 0, m_pGSBlob.GetAddressOf(), &pErrorBlob);
	}

	// �s�N�Z���V�F�[�_�[�̃R���p�C��
	{
		std::wstring fullFilepath = currentPath + filePath + L"_PS" + format;
		auto result = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, "main",
			"ps_5_0", flag, 0, m_pPSBlob.GetAddressOf(), &pErrorBlob);

		if (FAILED(result))
		{
			assert(0 && "�s�N�Z���V�F�[�_�[�̃R���p�C���Ɏ��s���܂���");
			return;
		}
	}
}

void Shader::SetMaterial(const Material& material) const
{
	material.spBaseColorTex->Set(m_cbvCount);
	material.spNormalTex->Set(m_cbvCount + 1);
	material.spMetallicRoughnessTex->Set(m_cbvCount + 2);
	material.spEmissiveTex->Set(m_cbvCount + 3);
}
