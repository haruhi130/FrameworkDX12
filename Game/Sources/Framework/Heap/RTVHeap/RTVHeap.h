#pragma once

class RTVHeap : public BaseHeap<int>
{
public: 
	RTVHeap(){}
	~RTVHeap(){}

	// RTV�쐬
	int CreateRTV(ID3D12Resource* pBuffer);
};