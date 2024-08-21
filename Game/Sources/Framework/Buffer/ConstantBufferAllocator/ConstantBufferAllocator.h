#pragma once

class ConstantBufferAllocator:public Buffer
{
public:
	~ConstantBufferAllocator()override{}

	// �萔�o�b�t�@�A���P�[�^�[�쐬
	void Create(CBVSRVUAVHeap* pHeap);

	// �g�p���Ă���o�b�t�@�ԍ���������
	void ResetCurrentUseNumber();

	// �萔�o�b�t�@�Ƀf�[�^���o�C���h
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

	// data�T�C�Y��256�A���C�����g���Čv�Z
	int sizeAligned = (sizeof(type) + 0xff) & ~0xff;

	// �g�p�����v�Z
	int useValue = sizeAligned / 0x100;

	// �g�p�ς݂̔ԍ��Ǝg�p�\��̗e�ʂ��q�[�v�e�ʂ𒴂��Ă�����return
	if (m_currentNumber + useValue > (int)m_pHeap->GetUseCount().x)
	{
		assert(0 && "�g�p�ł���q�[�v�e�ʂ��I�[�o�[");
		return;
	}
	
	int top = m_currentNumber;

	// �擪�A�h���X�Ɏg�p���镪�̃|�C���^�𑫂��ăR�s�[
	memcpy(m_pMappedBuffer + top, &data, sizeof(type));

	// �l���V�F�[�_�[�ɃA�^�b�`
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