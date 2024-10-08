#pragma once

class Texture : public Buffer
{
public:
	~Texture()override{}

	// �e�N�X�`�����[�h
	bool Load(const std::string& filePath);

	// �V�F�[�_�[���\�[�X�Ƃ��Đݒ�
	void Set(int index) const;

	// SRV�ԍ��擾
	inline int GetSRVNumber() const 
	{ return m_SRVNumber; }

	inline const D3D12_RESOURCE_DESC& GetInfo() const
	{ return m_desc; }

private:
	int m_SRVNumber = 0;

	D3D12_RESOURCE_DESC m_desc = {};
};