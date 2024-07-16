#pragma once

class CBVSRVUAVHeap : public BaseHeap<Math::Vector3>
{
public:
	CBVSRVUAVHeap(){}
	~CBVSRVUAVHeap(){}

	// SRV�쐬
	int CreateSRV(ID3D12Resource* pBuffer);

	// �q�[�v�w��
	void SetHeap();

	// GPU���A�h���X�擾
	const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(int number)override;

	// �q�[�v�擾
	inline ID3D12DescriptorHeap* GetHeap()
	{ return m_cpHeap.Get(); }

	// �q�[�v�g�p�ʎ擾
	inline const Math::Vector3& GetUseCount() 
	{ return m_useCount; }

};