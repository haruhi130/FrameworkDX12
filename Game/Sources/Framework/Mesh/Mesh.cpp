#include "Mesh.h"

void Mesh::Create()
{
	// 頂点バッファ作成
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
		assert(0 && "頂点バッファ作成失敗");
	}

	// 頂点バッファデータをビューに書き込み
	m_vbView.BufferLocation = m_cpVBuffer->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = (UINT)resDesc.Width;
	m_vbView.StrideInBytes = sizeof(Vertex);

	// インデックスバッファ作成
	// インデックスデータ
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
		assert(0 && "インデックスバッファ作成失敗");
	}

	// インデックスバッファデータをビューに書き込み
	m_ibView.BufferLocation = m_cpIBuffer->GetGPUVirtualAddress();
	m_ibView.SizeInBytes = (UINT)resDesc.Width;
	m_ibView.Format = DXGI_FORMAT_R32_UINT;

	// 頂点情報コピー
	Vertex* vbMap = nullptr;
	{
		m_cpVBuffer->Map(0, nullptr, (void**)&vbMap);
		std::copy(m_vertices.begin(), m_vertices.end(), vbMap);
		m_cpVBuffer->Unmap(0, nullptr);
	}

	// インデックス情報コピー
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
