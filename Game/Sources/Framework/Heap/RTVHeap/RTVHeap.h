#pragma once

class RTVHeap : public BaseHeap<int>
{
public: 
	RTVHeap(){}
	~RTVHeap(){}

	// RTVì¬
	int CreateRTV(ID3D12Resource* pBuffer);
};