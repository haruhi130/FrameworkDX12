#include "Mesh.h"

void Mesh::Create()
{
	// ���_�o�b�t�@�쐬
	m_vertices.emplace_back(Vertex({ -0.75f, -0.75f, 0.0f }, {0.0f, 1.0f}));
	m_vertices.emplace_back(Vertex({ -0.75f, 0.75f, 0.0f }, { 0.0f, 0.0f }));
	m_vertices.emplace_back(Vertex({ 0.75f, -0.75f, 0.0f }, { 1.0f, 1.0f }));
	m_vertices.emplace_back(Vertex({ 0.75f, 0.75f, 0.0f }, { 1.0f, 0.0f }));

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeof(Vertex) * m_vertices.size();
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	auto result = GraphicsDevice::GetInstance().GetDevice()->
		CreateCommittedResource(
			&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_cpVBuffer.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		assert(0 && "���_�o�b�t�@�쐬���s");
	}

	// ���_�o�b�t�@�f�[�^���r���[�ɏ�������
	m_vbView.BufferLocation = m_cpVBuffer->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = (UINT)resDesc.Width;
	m_vbView.StrideInBytes = sizeof(Vertex);

	// �C���f�b�N�X�o�b�t�@�쐬
	// �C���f�b�N�X�f�[�^
	m_indices.emplace_back(0);
	m_indices.emplace_back(1);
	m_indices.emplace_back(2);
	m_indices.emplace_back(2);
	m_indices.emplace_back(1);
	m_indices.emplace_back(3);

	resDesc.Width = sizeof(UINT) * m_indices.size();

	result = GraphicsDevice::GetInstance().GetDevice()->
		CreateCommittedResource(
			&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_cpIBuffer.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		assert(0 && "�C���f�b�N�X�o�b�t�@�쐬���s");
	}

	// �C���f�b�N�X�o�b�t�@�f�[�^���r���[�ɏ�������
	m_ibView.BufferLocation = m_cpIBuffer->GetGPUVirtualAddress();
	m_ibView.SizeInBytes = (UINT)resDesc.Width;
	m_ibView.Format = DXGI_FORMAT_R32_UINT;

	// ���_���R�s�[
	Vertex* vbMap = nullptr;
	{
		m_cpVBuffer->Map(0, nullptr, (void**)&vbMap);
		std::copy(m_vertices.begin(), m_vertices.end(), vbMap);
		m_cpVBuffer->Unmap(0, nullptr);
	}

	// �C���f�b�N�X���R�s�[
	UINT* ibMap = nullptr;
	{
		m_cpIBuffer->Map(0, nullptr, (void**)&ibMap);
		std::copy(m_indices.begin(), m_indices.end(), ibMap);
		m_cpIBuffer->Unmap(0, nullptr);
	}
}

void Mesh::DrawInstanced() const
{
	GraphicsDevice::GetInstance().GetCmdList()->IASetVertexBuffers(0, 1, &m_vbView);
	GraphicsDevice::GetInstance().GetCmdList()->IASetIndexBuffer(&m_ibView);
	GraphicsDevice::GetInstance().GetCmdList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
