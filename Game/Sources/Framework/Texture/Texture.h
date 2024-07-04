#pragma once

class Texture
{
public:
	// テクスチャロード
	bool Load(const std::string& filePath);

	// シェーダーリソースとして設定
	void Set(int index) const;

	// SRV番号取得
	inline int GetSRVNumber() const { return m_SRVNumber; }

private:
	ComPtr<ID3D12Resource> m_cpBuffer = nullptr;
	int m_SRVNumber = 0;

};