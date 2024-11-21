#pragma once

struct RenderTargetChange
{
	// �����_�[�^�[�Q�b�g�؂�ւ��p�̃��\�[�X�쐬
	bool CreateRenderTarget();

	// �����_�[�^�[�Q�b�g�ύX
	bool ChangeRenderTarget() const;
	// �����_�[�^�[�Q�b�g��߂�
	void UndoRenderTarget() const;

	// �����_�[�^�[�Q�b�g�p�e�N�X�`���쐬
	bool CreateRTTexture();

	bool CreateDepthTexture();

	// �`��
	void Draw() const;

	std::shared_ptr<Texture> m_spRTTexture = nullptr;
	int m_rtvNum = 0;

	int m_renderTargetSRVNumber = 0;

	std::shared_ptr<Texture> m_spDepthTexture = nullptr;
	int m_depthSRVNumber = 0;
};