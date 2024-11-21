#pragma once

class Buffer
{
public:
	Buffer(){}
	virtual ~Buffer(){}

	inline ComPtr<ID3D12Resource> GetBuffer()
	{
		return m_cpBuffer;
	}

protected:
	ComPtr<ID3D12Resource> m_cpBuffer = nullptr;
};