#include "ShaderBase.h"

bool ShaderBase::Init()
{
	m_cbvCount = 0;

	m_spRootSignature = std::make_shared<RootSignature>();
	m_spPipeline = std::make_shared<Pipeline>();
	
	return true;
}

void ShaderBase::Create(const std::shared_ptr<RootSignature>& spRootSignature, const std::shared_ptr<Pipeline>& spPipeline, const std::wstring& filePath,
	const RenderingSetting& renderingSetting, const std::vector<RangeType>& rangeTypes)
{
	// �V�F�[�_�[�ǂݍ���
	LoadShaderFile(filePath);

	// ���[�g�V�O�l�`���쐬
	spRootSignature->Create(rangeTypes, m_cbvCount);

	// �p�C�v���C���ݒ�
	spPipeline->SetRenderSettings(spRootSignature.get(), renderingSetting.InputLayouts,
		renderingSetting.CullMode, renderingSetting.BlendMode, renderingSetting.PrimitiveTopologyType);
	// �p�C�v���C���쐬
	spPipeline->Create({ m_cpVSBlob.Get() ,m_cpHSBlob.Get() ,m_cpDSBlob.Get() ,m_cpGSBlob.Get() ,m_cpPSBlob.Get() }, renderingSetting.Formats,
		renderingSetting.IsDepth, renderingSetting.IsDepthMask, renderingSetting.RTVCount, renderingSetting.IsWireFrame);
}

void ShaderBase::Begin(const std::shared_ptr<RootSignature>& spRootSignature, const std::shared_ptr<Pipeline>& spPipeline,
	int w, int h)
{
	// �p�C�v���C���Z�b�g
	GraphicsDevice::GetInstance().GetCmdList()->SetPipelineState(spPipeline->GetPipeline());

	// ���[�g�V�O�l�`���Z�b�g
	GraphicsDevice::GetInstance().GetCmdList()->SetGraphicsRootSignature(spRootSignature->GetRootSignature());

	// �g�|���W�[�ݒ�
	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType =
		static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(spPipeline->GetTopologyType());

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
	viewport.MinDepth = D3D12_MIN_DEPTH;
	viewport.MaxDepth = D3D12_MAX_DEPTH;
	GraphicsDevice::GetInstance().GetCmdList()->RSSetViewports(1, &viewport);

	// ���N�g�ݒ�
	D3D12_RECT rect = {};
	rect.right = w;
	rect.bottom = h;
	GraphicsDevice::GetInstance().GetCmdList()->RSSetScissorRects(1, &rect);
}

void ShaderBase::LoadShaderFile(const std::wstring& filePath, const std::string& entryPoint)
{
	ID3DInclude* include = D3D_COMPILE_STANDARD_FILE_INCLUDE;
	UINT flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ID3DBlob* pErrorBlob = nullptr;

	std::wstring format = L".hlsl";
	std::wstring currentPath = L"Sources/Framework/Shader/HLSL/";

	// ���_�V�F�[�_�[�̃R���p�C��
	{
		std::wstring fullFilepath = currentPath + filePath + L"_VS" + format;
		auto result = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, entryPoint.data(),
			"vs_5_0", flag, 0, m_cpVSBlob.GetAddressOf(), &pErrorBlob);

		if (FAILED(result))
		{
			assert(0 && "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂���");
			return;
		}
	}

	// �n���V�F�[�_�[�̃R���p�C��
	{
		std::wstring fullFilepath = currentPath + filePath + L"_HS" + format;
		auto result = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, entryPoint.data(),
			"hs_5_0", flag, 0, m_cpHSBlob.GetAddressOf(), &pErrorBlob);
	}

	// �h���C���V�F�[�_�[�̃R���p�C��
	{
		std::wstring fullFilepath = currentPath + filePath + L"_DS" + format;
		auto result = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, entryPoint.data(),
			"ds_5_0", flag, 0, m_cpDSBlob.GetAddressOf(), &pErrorBlob);
	}

	// �W�I���g���V�F�[�_�[�̃R���p�C��
	{
		std::wstring fullFilepath = currentPath + filePath + L"_GS" + format;
		auto result = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, entryPoint.data(),
			"gs_5_0", flag, 0, m_cpGSBlob.GetAddressOf(), &pErrorBlob);
	}

	// �s�N�Z���V�F�[�_�[�̃R���p�C��
	{
		std::wstring fullFilepath = currentPath + filePath + L"_PS" + format;
		auto result = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, entryPoint.data(),
			"ps_5_0", flag, 0, m_cpPSBlob.GetAddressOf(), &pErrorBlob);

		if (FAILED(result))
		{
			assert(0 && "�s�N�Z���V�F�[�_�[�̃R���p�C���Ɏ��s���܂���");
			return;
		}
	}
}
