#pragma once

class Buffer
{
public:
	Buffer(){}
	virtual ~Buffer(){}

	/// <summary>
	/// �o�b�t�@�擾
	/// </summary>
	/// <returns>�g�p���Ă���o�b�t�@</returns>
	inline ComPtr<ID3D12Resource> GetBuffer()
	{ return m_cpBuffer; }

protected:
	ComPtr<ID3D12Resource> m_cpBuffer = nullptr;
};