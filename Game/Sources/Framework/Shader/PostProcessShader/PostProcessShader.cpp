#include "PostProcessShader.h"

bool PostProcessShader::Init()
{
	ShaderBase::Init();

	// �����_�[�^�[�Q�b�g�ύX�p���\�[�X�쐬
	if (!m_RTChange.CreateRenderTarget()) { return false; }
	if (!m_RTChange.CreateRTTexture()) { return false; }

	// �[�x�e�N�X�`���쐬
	if (!m_RTChange.CreateDepthTexture()) { return false; }
	
	// �V�F�[�_�[�ɓn�����ݒ�
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };
	renderingSetting.IsDepth = false;

	// �|�X�g�v���Z�X�`��V�F�[�_�[�쐬
	Create(m_spRootSignature, m_spPipeline, L"PostProcessShader/PostProcessShader"
		, renderingSetting, { RangeType::SRV });

	return true;
}

void PostProcessShader::Begin(int w, int h)
{
	// DescriptorHeap�ݒ�
	GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

	// ���[�g�V�O�l�`���ƃp�C�v���C���ݒ�
	ShaderBase::Begin(m_spRootSignature, m_spPipeline, w, h);

	// �ύX���������_�[�^�[�Q�b�g��ɏ������񂾉摜���V�F�[�_�[�ɃZ�b�g
	m_RTChange.m_spRTTexture->SetToShader(m_cbvCount);
}

void PostProcessShader::PreDraw()
{
	// �����_�[�^�[�Q�b�g�ύX
	m_RTChange.ChangeRenderTarget();
}

void PostProcessShader::Draw()
{
	// �`��
	m_RTChange.Draw();
}

void PostProcessShader::PostProcess()
{
	// �����_�[�^�[�Q�b�g�ύX
	m_RTChange.UndoRenderTarget();
}
