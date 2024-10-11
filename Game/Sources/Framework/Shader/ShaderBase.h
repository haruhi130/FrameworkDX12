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
	bool IsStrip = false;
};

class ShaderBase
{
public:
	virtual bool Init();

	// �V�F�[�_�[�쐬
	void Create(const std::wstring& filePath,
		const RenderingSetting& renderingSetting, const std::vector<RangeType>& rangeTypes);

	// �`��J�n
	virtual void Begin(int w = windowWidth, int h = windowHeight);

	// CBV�J�E���g�擾
	inline UINT GetCBVCount()const 
	{ return m_cbvCount; }

protected:
	// �V�F�[�_�[�t�@�C���Ǎ�
	void LoadShaderFile(const std::wstring& filePath);

	std::unique_ptr<Pipeline>		m_upPipeline = nullptr;
	std::unique_ptr<RootSignature>	m_upRootSignature = nullptr;

	ComPtr<ID3DBlob> m_pVSBlob = nullptr;	// ���_�V�F�[�_�[
	ComPtr<ID3DBlob> m_pHSBlob = nullptr;	// �n���V�F�[�_�[
	ComPtr<ID3DBlob> m_pDSBlob = nullptr;	// �h���C���V�F�[�_�[
	ComPtr<ID3DBlob> m_pGSBlob = nullptr;	// �W�I���g���V�F�[�_�[
	ComPtr<ID3DBlob> m_pPSBlob = nullptr;	// �s�N�Z���V�F�[�_�[

	UINT m_cbvCount = 0;
};