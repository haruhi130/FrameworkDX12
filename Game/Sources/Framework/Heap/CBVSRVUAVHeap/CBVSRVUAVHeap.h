#pragma once

class CBVSRVUAVHeap : public BaseHeap<Math::Vector3>
{
public:
	CBVSRVUAVHeap(){}
	~CBVSRVUAVHeap()override{}

	// SRV�쐬
	int CreateSRV(ID3D12Resource* pBuffer);

	// GPU���A�h���X�擾
	const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(int number)override;

	// �q�[�v�w��
	void SetHeap();

};