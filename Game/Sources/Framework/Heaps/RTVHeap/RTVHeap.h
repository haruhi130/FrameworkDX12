#pragma once

class RTVHeap
{
public:
	// RTV用ディスクリプタヒープ作成
	bool Create();

	// ハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle();

private:
	ComPtr<ID3D12DescriptorHeap> m_cpRTVHeap = nullptr;
};