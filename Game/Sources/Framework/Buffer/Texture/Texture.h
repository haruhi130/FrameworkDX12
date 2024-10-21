#pragma once

class Texture : public Buffer
{
public:
	~Texture()override {}

	// テクスチャロード
	bool Load(const std::string& filePath);

	bool Create(const std::string& fileName, const Math::Vector2& pos, const Math::Rectangle* rect = nullptr, const Math::Vector2& pivot = { 0.5f,0.5f });

	bool CreateRenderTarget();

	// シェーダーリソースとして設定
	void Set(int index) const;

	void SetToDevice() const
	{
		GraphicsDevice::GetInstance().GetCmdList()->IASetVertexBuffers(0, 1, &m_VBV);
		GraphicsDevice::GetInstance().GetCmdList()->IASetIndexBuffer(&m_IBV);
	}

	// SRV番号取得
	inline int GetSRVNumber() const
	{
		return m_SRVNumber;
	}

	inline const D3D12_RESOURCE_DESC& GetInfo() const
	{
		return m_desc;
	}

private:
	int m_SRVNumber = 0;

	D3D12_RESOURCE_DESC m_desc = {};

	ComPtr<ID3D12Resource> m_VB = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_VBV = {};

	ComPtr<ID3D12Resource> m_IB = nullptr;
	D3D12_INDEX_BUFFER_VIEW m_IBV = {};
};