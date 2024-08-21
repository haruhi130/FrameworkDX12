#pragma once

class RTVHeap : public BaseHeap<int>
{
public: 
	RTVHeap(){}
	~RTVHeap()override{}

	// RTVì¬
	int CreateRTV(ID3D12Resource* pBuffer);
};