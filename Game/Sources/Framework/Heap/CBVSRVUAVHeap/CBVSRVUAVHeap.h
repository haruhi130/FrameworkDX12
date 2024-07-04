#pragma once

class CBVSRVUAVHeap : public BaseHeap<Math::Vector3>
{
public:
	CBVSRVUAVHeap(){}
	~CBVSRVUAVHeap()override{}

	// SRV作成
	int CreateSRV(ID3D12Resource* pBuffer);

	// GPU側アドレス取得
	const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(int number)override;

	// ヒープ指定
	void SetHeap();

};