#pragma once

class Texture
{
public:
	// �e�N�X�`�����[�h
	bool Load(const std::string& filePath);

	// �V�F�[�_�[���\�[�X�Ƃ��Đݒ�
	void Set(int index) const;

	// SRV�ԍ��擾
	inline int GetSRVNumber() const { return m_SRVNumber; }

private:
	ComPtr<ID3D12Resource> m_cpBuffer = nullptr;
	int m_SRVNumber = 0;

};