#pragma once

class DSVHeap :public BaseHeap<int>
{
public:
	DSVHeap(){}
	~DSVHeap(){}

	// DSV�쐬
	int CreateDSV(ID3D12Resource* pBuffer, DXGI_FORMAT format);
};