#pragma once

class RTVHeap : public BaseHeap<int>
{
public: 
	RTVHeap(){}
	~RTVHeap()override{}

	// RTV�쐬
	int CreateRTV(ID3D12Resource* pBuffer);
};