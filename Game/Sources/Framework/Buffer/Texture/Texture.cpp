#include "Texture.h"

bool Texture::Load(const std::string& filePath)
{
	wchar_t wFilePath[128];
	MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, wFilePath, _countof(wFilePath));

	DirectX::TexMetadata metadata = {};
	DirectX::ScratchImage scratchImage = {};
	const DirectX::Image* pImage = nullptr;

	auto result = DirectX::LoadFromWICFile(wFilePath, DirectX::WIC_FLAGS_NONE, &metadata, scratchImage);
	if (FAILED(result))
	{
		assert(0 && "テクスチャの読み込み失敗");
		return false;
	}

	pImage = scratchImage.GetImage(0, 0, 0);

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	resDesc.Format = metadata.format;
	resDesc.Width = (UINT64)metadata.width;
	resDesc.Height = (UINT)metadata.height;
	resDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	resDesc.MipLevels = (UINT16)metadata.mipLevels;
	resDesc.SampleDesc.Count = 1;

	result = GraphicsDevice::GetInstance().GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_cpBuffer.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		assert(0 && "テクスチャバッファ作成失敗");
		return false;
	}

	result = m_cpBuffer->WriteToSubresource(0, nullptr, pImage->pixels, (UINT)pImage->rowPitch, (UINT)pImage->slicePitch);
	if (FAILED(result))
	{
		assert(0 && "バッファテクスチャデータ書き込み失敗");
		return false;
	}

	m_desc = resDesc;

	m_SRVNumber = GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->CreateSRV(m_cpBuffer.Get());

	return true;
}

bool Texture::CreateRenderTarget()
{
	Vertex vertex[] =
	{
		{{-1,-1, 0.1f},{0,1}},
		{{-1, 1, 0.1f},{0,0}},
		{{ 1,-1, 0.1f},{1,1}},
		{{ 1, 1, 0.1f},{1,0}}
	};

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeof(vertex);
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	auto result = GraphicsDevice::GetInstance().GetDevice()
		->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE,
			&resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_VB.ReleaseAndGetAddressOf()));

	Vertex* vbMap = nullptr;
	m_VB->Map(0, nullptr, (void**)&vbMap);
	std::copy(std::begin(vertex), std::end(vertex), vbMap);
	m_VB->Unmap(0, nullptr);

	m_VBV.BufferLocation = m_VB->GetGPUVirtualAddress();
	m_VBV.SizeInBytes = sizeof(vertex);
	m_VBV.StrideInBytes = sizeof(Vertex);

	std::vector<UINT> indices;
	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(2);
	indices.emplace_back(1);
	indices.emplace_back(3);

	resDesc.Width = sizeof(UINT) * indices.size();

	result = GraphicsDevice::GetInstance().GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_IB.ReleaseAndGetAddressOf()));

	UINT* ibMap = nullptr;
	m_IB->Map(0, nullptr, (void**)&ibMap);
	std::copy(std::begin(indices), std::end(indices), ibMap);
	m_IB->Unmap(0, nullptr);

	m_IBV.BufferLocation = m_IB->GetGPUVirtualAddress();
	m_IBV.Format = DXGI_FORMAT_R32_UINT;
	m_IBV.SizeInBytes = (UINT)resDesc.Width;

	return true;
}

void Texture::Set(int index) const
{
	GraphicsDevice::GetInstance().GetCmdList()->SetGraphicsRootDescriptorTable(
		index, GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->GetGPUHandle(m_SRVNumber));
}
