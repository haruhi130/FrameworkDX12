#pragma once

class RenderTargetChange
{
public:
	bool CreateRenderTarget();

	bool ChangeRenderTarget();
	void UndoRenderTarget();

	bool CreateRTTexture();

	void Draw();

private:

	ComPtr<ID3D12Resource> m_cpResource = nullptr;
	ComPtr<ID3D12DescriptorHeap> m_cpRTVResource = nullptr;
	ComPtr<ID3D12DescriptorHeap> m_cpSRVResource = nullptr;
	
	std::shared_ptr<Texture> m_spRTTexture = nullptr;
};