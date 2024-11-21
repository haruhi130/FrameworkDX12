#pragma once

#include "Pipeline/Pipeline.h"
#include "RootSignature/RootSignature.h"

struct RenderingSetting
{
	std::vector<InputLayout> InputLayouts;			// ���_���C�A�E�g
	std::vector<DXGI_FORMAT> Formats;				// �t�H�[�}�b�g
	CullMode CullMode = CullMode::Back;				// �J�����O�ݒ�(�f�t�H���g:�w�ʃJ�����O)
	BlendMode BlendMode = BlendMode::Alpha;			// �u�����h�ݒ�(�f�t�H���g:�ʏ퍇��)
	PrimitiveTopologyType PrimitiveTopologyType 
				= PrimitiveTopologyType::Triangle;	// �g�|���W�[�ݒ�(�f�t�H���g:�O�p�`)
	bool IsDepth = true;							// �[�x�g�p�t���O(�f�t�H���g:�g�p)
	bool IsDepthMask = true;
	int RTVCount = 1;								// �����_�[�^�[�Q�b�g��(�f�t�H���g:1)
	bool IsWireFrame = false;
};

class ShaderBase
{
public:
	// ������
	virtual bool Init();

	// �V�F�[�_�[�쐬
	void Create(const std::shared_ptr<RootSignature>& spRootSignature, const std::shared_ptr<Pipeline>& spPipeline,
		const std::wstring& filePath,const RenderingSetting& renderingSetting, const std::vector<RangeType>& rangeTypes);

	// �`��J�n
	virtual void Begin(const std::shared_ptr<RootSignature>& spRootSignature, const std::shared_ptr<Pipeline>& spPipeline,
		int w = window_width, int h = window_height);

	// CBV�J�E���g�擾
	inline UINT GetCBVCount() const 
	{ return m_cbvCount; }

protected:
	// �V�F�[�_�[�t�@�C���Ǎ�
	void LoadShaderFile(const std::wstring& filePath,const std::string& entryPoint = "main");

	// ���[�g�V�O�l�`��
	std::shared_ptr<RootSignature>	m_spRootSignature = nullptr;
	// �p�C�v���C��
	std::shared_ptr<Pipeline>		m_spPipeline = nullptr;

	ComPtr<ID3DBlob> m_cpVSBlob = nullptr;	// ���_�V�F�[�_�[
	ComPtr<ID3DBlob> m_cpHSBlob = nullptr;	// �n���V�F�[�_�[
	ComPtr<ID3DBlob> m_cpDSBlob = nullptr;	// �h���C���V�F�[�_�[
	ComPtr<ID3DBlob> m_cpGSBlob = nullptr;	// �W�I���g���V�F�[�_�[
	ComPtr<ID3DBlob> m_cpPSBlob = nullptr;	// �s�N�Z���V�F�[�_�[

	UINT m_cbvCount = 0;
};