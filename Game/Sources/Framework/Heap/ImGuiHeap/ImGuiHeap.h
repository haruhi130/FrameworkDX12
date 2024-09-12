#pragma once

class ImGuiHeap : public BaseHeap<int>
{
public:
	ImGuiHeap(){}
	~ImGuiHeap()override{}

	// ƒq[ƒvæ“¾
	inline ComPtr<ID3D12DescriptorHeap> GetHeap()
	{
		return m_cpHeap;
	}
};