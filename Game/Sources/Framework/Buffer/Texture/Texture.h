#pragma once

class Texture:public Buffer
{
public:
	~Texture(){}

	// �e�N�X�`�����[�h
	bool Load(const std::string& filePath);

	// �V�F�[�_�[���\�[�X�Ƃ��Đݒ�
	void Set(int index) const;

	// SRV�ԍ��擾
	inline int GetSRVNumber() const { return m_SRVNumber; }

private:
	int m_SRVNumber = 0;

};