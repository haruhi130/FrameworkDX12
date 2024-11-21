#pragma once

class PostProcessShader : public ShaderBase
{
public:
	// ������
	bool Init()override;

	// �V�F�[�_�[�ݒ�
	void Begin(int w = window_width, int h = window_height);

	// �����_�[�^�[�Q�b�g�ύX
	void PreDraw();

	// �`��
	void Draw();

	// �����_�[�^�[�Q�b�g�ύX
	void PostProcess();

private:
	// �����_�[�^�[�Q�b�g�ύX�p
	RenderTargetChange m_RTChange;
};