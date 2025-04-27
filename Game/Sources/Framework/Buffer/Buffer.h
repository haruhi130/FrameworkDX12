#pragma once

class Buffer
{
public:
	Buffer(){}
	virtual ~Buffer(){}

	/// <summary>
	/// バッファ取得
	/// </summary>
	/// <returns>使用しているバッファ</returns>
	inline ComPtr<ID3D12Resource> GetBuffer()
	{ return m_cpBuffer; }

protected:
	ComPtr<ID3D12Resource> m_cpBuffer = nullptr;
};