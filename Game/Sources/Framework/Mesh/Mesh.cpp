#include "Mesh.h"

void Mesh::Create(const std::vector<MeshVertex>& vertices,
	const std::vector<MeshFace>& faces, const Material& material)
{
	m_material = material;

	if (static_cast<int>(vertices.size()) == 0)
	{
		assert(0 && "頂点が存在しません");
		return;
	}

	m_instanceCount = static_cast<UINT>(faces.size() * 3);

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeof(MeshVertex) * vertices.size();
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファ作成
	auto result = GraphicsDevice::GetInstance().GetDevice()->
		CreateCommittedResource(
			&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_cpVBuffer.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		assert(0 && "頂点バッファ作成失敗");
	}

	m_positions.resize(vertices.size());
	for (UINT i = 0; i < m_positions.size(); i++)
	{
		m_positions[i] = vertices[i].Position;
	}

	DirectX::BoundingBox::CreateFromPoints(m_aabb, m_positions.size(), &m_positions[0], sizeof(Math::Vector3));
	DirectX::BoundingSphere::CreateFromPoints(m_bs, m_positions.size(), &m_positions[0], sizeof(Math::Vector3));

	// 頂点バッファデータをビューに書き込み
	m_vbView.BufferLocation = m_cpVBuffer->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = (UINT)resDesc.Width;
	m_vbView.StrideInBytes = sizeof(MeshVertex);

	resDesc.Width = sizeof(MeshFace) * faces.size();

	// インデックスバッファ作成
	result = GraphicsDevice::GetInstance().GetDevice()->
		CreateCommittedResource(
			&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_cpIBuffer.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		assert(0 && "インデックスバッファ作成失敗");
	}

	m_faces = faces;

	// インデックスバッファデータをビューに書き込み
	m_ibView.BufferLocation = m_cpIBuffer->GetGPUVirtualAddress();
	m_ibView.SizeInBytes = (UINT)resDesc.Width;
	m_ibView.Format = DXGI_FORMAT_R32_UINT;

	// 頂点情報コピー
	MeshVertex* vbMap = nullptr;
	{
		m_cpVBuffer->Map(0, nullptr, (void**)&vbMap);
		std::copy(std::begin(vertices), std::end(vertices), vbMap);
		m_cpVBuffer->Unmap(0, nullptr);
	}

	// インデックス情報コピー
	MeshFace* ibMap = nullptr;
	{
		m_cpIBuffer->Map(0, nullptr, (void**)&ibMap);
		std::copy(std::begin(faces), std::end(faces), ibMap);
		m_cpIBuffer->Unmap(0, nullptr);
	}
}

void Mesh::DrawInstanced(UINT vertexCount) const
{
	GraphicsDevice::GetInstance().GetCmdList()->IASetVertexBuffers(0, 1, &m_vbView);
	GraphicsDevice::GetInstance().GetCmdList()->IASetIndexBuffer(&m_ibView);

	GraphicsDevice::GetInstance().GetCmdList()->DrawIndexedInstanced(vertexCount, 1, 0, 0, 0);
}
