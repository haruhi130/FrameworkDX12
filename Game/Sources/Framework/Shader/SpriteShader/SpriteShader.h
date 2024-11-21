#pragma once
#include "../ShaderBase.h"

class SpriteShader : public ShaderBase
{
public:
	// ������
	bool Init()override;

	// �V�F�[�_�[�ݒ�
	void Begin(int w = window_width, int h = window_height);

	// �摜�`��
	void DrawTexture(const Texture* tex,const Mesh* mesh,const Math::Color& color = { 1,1,1,1 });

	// �s��ݒ�
	inline void SetMatrix(const Math::Matrix& m)
	{  m_cbSprite.mTransform = m; }

private:
	// �萔�o�b�t�@
	ConstantBufferData::SpriteInfo m_cbSprite;
	ConstantBufferData::ProjectionInfo m_cbProj;
};