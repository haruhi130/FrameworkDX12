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

	m_SRVNumber = GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->CreateSRV(m_cpBuffer.Get());

	return true;
}

void Texture::Set(int index) const
{
	GraphicsDevice::GetInstance().GetCmdList()->SetGraphicsRootDescriptorTable(
		index, GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->GetGPUHandle(m_SRVNumber));
}
