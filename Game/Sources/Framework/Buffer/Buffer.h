#pragma once

class Buffer
{
public:
	Buffer(){}
	virtual ~Buffer(){}

protected:
	ComPtr<ID3D12Resource> m_cpBuffer = nullptr;
};