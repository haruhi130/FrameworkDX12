#pragma once

class Texture : public Buffer
{
public:
	~Texture()override {}

	/// <summary>
	/// �t�@�C���p�X����e�N�X�`����ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C���p�X</param>
	/// <returns>�ǂݍ��݂ɐ���������true</returns>
	bool Load(std::string_view fileName);

	/// <summary>
	/// �}���`�p�X�����_�����O�Ɏg�p���郊�\�[�X�쐬
	/// </summary>
	/// <returns>���\�[�X�쐬�ɐ���������true</returns>
	bool CreateMultiPassResource();

	/// <summary>
	/// �����_�[�^�[�Q�b�g�Ɏg�p����|���쐬
	/// </summary>
	/// <returns>�|���쐬�ɐ���������true</returns>
	bool CreateRenderTarget();
	
	// �[�x�e�N�X�`���쐬
	
	/// <summary>
	/// �[�x�o�b�t�@����SRV�쐬
	/// </summary>
	/// <returns>�쐬�ɐ���������true</returns>
	bool CreateDepthSRV();
	/// <summary>
	/// �����[�x�o�b�t�@����SRV�쐬
	/// </summary>
	/// <returns>�쐬�ɐ���������true</returns>
	bool CreateLightDepthSRV();

	/// <summary>
	/// �V�F�[�_�[���\�[�X�Ƃ��ăZ�b�g
	/// </summary>
	/// <param name="index">�g�p����C���f�b�N�X�ԍ�</param>
	void SetToShader(int index) const;

	/// <summary>
	/// �|���𒸓_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�ɃZ�b�g
	/// </summary>
	void SetToDevice() const
	{
		GraphicsDevice::GetInstance().GetCmdList()->IASetVertexBuffers(0, 1, &m_VBV);
		GraphicsDevice::GetInstance().GetCmdList()->IASetIndexBuffer(&m_IBV);
	}

	/// <summary>
	/// SRV�ԍ����擾
	/// </summary>
	/// <returns>SRV�ԍ�</returns>
	inline int GetSRVNumber() const
	{ return m_SRVNumber; }

	/// <summary>
	/// �e�N�X�`���̃��\�[�X�f�X�N�����擾
	/// </summary>
	/// <returns>���\�[�X�f�X�N���</returns>
	inline const D3D12_RESOURCE_DESC& GetInfo() const
	{ return m_desc; }

private:
	int m_SRVNumber = 0;

	D3D12_RESOURCE_DESC m_desc = {};

	ComPtr<ID3D12Resource> m_VB = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_VBV = {};

	ComPtr<ID3D12Resource> m_IB = nullptr;
	D3D12_INDEX_BUFFER_VIEW m_IBV = {};
};