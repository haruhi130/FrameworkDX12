#pragma once

class RTVHeap
{
public:
	// RTV�p�f�B�X�N���v�^�q�[�v�쐬
	bool Create();

	// �n���h���擾
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle();

private:
	ComPtr<ID3D12DescriptorHeap> m_cpRTVHeap = nullptr;
};