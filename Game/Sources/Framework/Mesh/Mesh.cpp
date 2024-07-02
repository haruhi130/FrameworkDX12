#include "Mesh.h"

void Mesh::Create()
{
	m_vertices[0] = { -1.0f,-1.0f };
	m_vertices[1] = { 0.0f,1.0f };
	m_vertices[2] = { 1.0f,-1.0f };


	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeof(Math::Vector3)* m_vertices.size();
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

	// 頂点情報コピー
	Math::Vector3* vbMap = nullptr;
	{
		m_cpVBuffer->Map(0, nullptr, (void**)&vbMap);
		std::copy(std::begin(m_vertices), std::end(m_vertices), vbMap);
		m_cpVBuffer->Unmap(0, nullptr);
	}
	
	m_vbView.BufferLocation = m_cpVBuffer->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = (UINT)resDesc.Width;
	m_vbView.StrideInBytes = sizeof(Math::Vector3);
}

void Mesh::DrawInstanced() const
{
	GraphicsDevice::GetInstance().GetCmdList()->IASetVertexBuffers(0, 1, &m_vbView);
	GraphicsDevice::GetInstance().GetCmdList()->DrawInstanced(3, 1, 0, 0);
}
