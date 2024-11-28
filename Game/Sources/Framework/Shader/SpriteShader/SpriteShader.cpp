#include "SpriteShader.h"

bool SpriteShader::Init()
{
	ShaderBase::Init();

	// �V�F�[�_�[�ɓn�����ݒ�
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };
	renderingSetting.IsDepth = false;

	// 2D�`��V�F�[�_�[�쐬
	Create(m_spRootSignature, m_spPipeline, L"SpriteShader/SpriteShader", renderingSetting,
		{ RangeType::CBV,RangeType::CBV,RangeType::SRV });

	return true;
}

void SpriteShader::Begin(int w, int h)
{
	// DescriptorHeap�ݒ�
	GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

	// ���[�g�V�O�l�`���ƃp�C�v���C���ݒ�
	ShaderBase::Begin(m_spRootSignature, m_spPipeline, w, h);

	// �ˉe�s��ݒ�
	m_cbProj.mProj = DirectX::XMMatrixOrthographicLH((float)w, (float)h, 0, 1);

	// �ˉe�s����]��
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(1, m_cbProj);
}

void SpriteShader::DrawTexture(const Texture* tex, const Mesh* mesh, const Math::Color& color)
{
	if (tex == nullptr) { return; }

	// �摜���V�F�[�_�[�ɃZ�b�g
	tex->Set(m_cbvCount);

	// �F���]��
	m_cbSprite.Color = color;
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(0, m_cbSprite);

	// �`��
	mesh->DrawIndexed();
}
