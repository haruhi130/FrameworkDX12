#pragma once

class ConstantBufferAllocator:public Buffer
{
public:
	~ConstantBufferAllocator()override{}

	// 定数バッファアロケーター作成
	void Create(CBVSRVUAVHeap* pHeap);

	// 使用しているバッファ番号を初期化
	void ResetCurrentUseNumber();

	// 定数バッファにデータをバインド
	template<typename type>
	void BindAndAttachData(int descIndex, const type& data);

private:
	CBVSRVUAVHeap* m_pHeap = nullptr;
	struct { char buf[256]; }*m_pMappedBuffer = nullptr;
	int m_currentNumber = 0;

};

template<typename type>
inline void ConstantBufferAllocator::BindAndAttachData(int descIndex, const type& data)
{
	if (!m_pHeap){ return; }

	// dataサイズを256アライメントして計算
	int sizeAligned = (sizeof(type) + 0xff) & ~0xff;

	// 使用数を計算
	int useValue = sizeAligned / 0x100;

	// 使用済みの番号と使用予定の容量がヒープ容量を超えていたらreturn
	if (m_currentNumber + useValue > (int)m_pHeap->GetUseCount().x)
	{
		assert(0 && "使用できるヒープ容量をオーバー");
		return;
	}
	
	int top = m_currentNumber;

	// 先頭アドレスに使用する分のポインタを足してコピー
	memcpy(m_pMappedBuffer + top, &data, sizeof(type));

	// 値をシェーダーにアタッチ
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc = {};
	cbDesc.BufferLocation = m_cpBuffer->GetGPUVirtualAddress() + (UINT64)top * 0x100;
	cbDesc.SizeInBytes = sizeAligned;

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = m_pHeap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	cpuHandle.ptr += (UINT64)GraphicsDevice::GetInstance().GetDevice()->GetDescriptorHandleIncrementSize
	(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * m_currentNumber;

	GraphicsDevice::GetInstance().GetDevice()->CreateConstantBufferView(&cbDesc, cpuHandle);

	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = m_pHeap->GetHeap()->GetGPUDescriptorHandleForHeapStart();
	gpuHandle.ptr += (UINT64)GraphicsDevice::GetInstance().GetDevice()->GetDescriptorHandleIncrementSize
	(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * m_currentNumber;

	GraphicsDevice::GetInstance().GetCmdList()->SetGraphicsRootDescriptorTable(descIndex, gpuHandle);

	m_currentNumber += useValue;
}