#include "Mesh.h"

void Mesh::SetToDevice() const
{
	GraphicsDevice::GetInstance().GetCmdList()->IASetVertexBuffers(0, 1, &m_vbView);
	GraphicsDevice::GetInstance().GetCmdList()->IASetIndexBuffer(&m_ibView);
}

void Mesh::Create(const std::vector<MeshVertex>& vertices,
	const std::vector<MeshFace>& faces, const std::vector<MeshSubset>& subsets, bool isSkinMesh)
{
	m_subsets = subsets;

	if (static_cast<int>(vertices.size()) == 0)
	{
		assert(0 && "頂点が存在しません");
		return;
	}

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

	m_faces = faces;
	m_isSkinMesh = isSkinMesh;
}

void Mesh::Create(std::vector<Vertex> vertices)
{
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeof(Vertex) * vertices.size();
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

	// 頂点バッファデータをビューに書き込み
	m_vbView.BufferLocation = m_cpVBuffer->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = (UINT)resDesc.Width;
	m_vbView.StrideInBytes = sizeof(Vertex);

	std::vector<UINT> indices;
	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(2);
	indices.emplace_back(1);
	indices.emplace_back(3);

	resDesc.Width = sizeof(UINT) * indices.size();

	// インデックスバッファ作成
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
		std::copy(std::begin(vertices), std::end(vertices), vbMap);
		m_cpVBuffer->Unmap(0, nullptr);
	}

	// インデックス情報コピー
	UINT* ibMap = nullptr;
	{
		m_cpIBuffer->Map(0, nullptr, (void**)&ibMap);
		std::copy(std::begin(indices), std::end(indices), ibMap);
		m_cpIBuffer->Unmap(0, nullptr);
	}
}

void Mesh::DrawSubset(int subsetNo) const
{
	if (subsetNo >= (int)m_subsets.size())return;
	if (m_subsets[subsetNo].FaceCount == 0)return;

	GraphicsDevice::GetInstance().GetCmdList()->DrawIndexedInstanced(
		m_subsets[subsetNo].FaceCount * 3,
		1,
		m_subsets[subsetNo].FaceStart * 3,
		0,
		0
	);
}

void Mesh::DrawInstanced(UINT vertexCount) const
{
	GraphicsDevice::GetInstance().GetCmdList()->IASetVertexBuffers(0, 1, &m_vbView);
	GraphicsDevice::GetInstance().GetCmdList()->IASetIndexBuffer(&m_ibView);

	GraphicsDevice::GetInstance().GetCmdList()->DrawIndexedInstanced(vertexCount, 1, 0, 0, 0);
}
