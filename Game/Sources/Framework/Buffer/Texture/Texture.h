#pragma once

class Texture : public Buffer
{
public:
	~Texture()override{}

	// テクスチャロード
	bool Load(const std::string& filePath);

	// シェーダーリソースとして設定
	void Set(int index) const;

	// SRV番号取得
	inline int GetSRVNumber() const 
	{ return m_SRVNumber; }

	inline const D3D12_RESOURCE_DESC& GetInfo() const
	{ return m_desc; }

private:
	int m_SRVNumber = 0;

	D3D12_RESOURCE_DESC m_desc = {};
};