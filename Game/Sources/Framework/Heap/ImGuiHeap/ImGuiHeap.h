#pragma once

class ImGuiHeap : public BaseHeap<int>
{
public:
	ImGuiHeap(){}
	~ImGuiHeap()override{}

	// �q�[�v�擾
	inline ComPtr<ID3D12DescriptorHeap> GetHeap()
	{
		return m_cpHeap;
	}
};