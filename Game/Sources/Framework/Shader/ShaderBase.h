#pragma once

#include "Pipeline/Pipeline.h"
#include "RootSignature/RootSignature.h"

struct RenderingSetting
{
	std::vector<InputLayout> InputLayouts;
	std::vector<DXGI_FORMAT> Formats;
	CullMode CullMode = CullMode::Back;
	BlendMode BlendMode = BlendMode::Alpha;
	PrimitiveTopologyType PrimitiveTopologyType = PrimitiveTopologyType::Triangle;
	bool IsDepth = true;
	bool IsDepthMask = true;
	int RTVCount = 1;
	bool IsWireFrame = false;
};

class ShaderBase
{
public:
	virtual bool Init();

	// �V�F�[�_�[�쐬
	void Create(std::shared_ptr<RootSignature> spRootSignature, std::shared_ptr<Pipeline> spPipeline,const std::wstring& filePath,
		const RenderingSetting& renderingSetting, const std::vector<RangeType>& rangeTypes);

	// �`��J�n
	virtual void Begin(std::shared_ptr<RootSignature> spRootSignature, std::shared_ptr<Pipeline> spPipeline,
		int w = windowWidth, int h = windowHeight);

	// CBV�J�E���g�擾
	inline UINT GetCBVCount()const 
	{ return m_cbvCount; }

protected:
	// �V�F�[�_�[�t�@�C���Ǎ�
	void LoadShaderFile(const std::wstring& filePath,const std::string& entryPoint = "main");

	std::shared_ptr<Pipeline>		m_spPipeline = nullptr;
	std::shared_ptr<RootSignature>	m_spRootSignature = nullptr;

	ComPtr<ID3DBlob> m_cpVSBlob = nullptr;	// ���_�V�F�[�_�[
	ComPtr<ID3DBlob> m_cpHSBlob = nullptr;	// �n���V�F�[�_�[
	ComPtr<ID3DBlob> m_cpDSBlob = nullptr;	// �h���C���V�F�[�_�[
	ComPtr<ID3DBlob> m_cpGSBlob = nullptr;	// �W�I���g���V�F�[�_�[
	ComPtr<ID3DBlob> m_cpPSBlob = nullptr;	// �s�N�Z���V�F�[�_�[

	UINT m_cbvCount = 0;
};