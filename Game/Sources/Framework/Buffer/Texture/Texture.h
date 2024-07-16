#pragma once

class Texture:public Buffer
{
public:
	~Texture(){}

	// テクスチャロード
	bool Load(const std::string& filePath);

	// シェーダーリソースとして設定
	void Set(int index) const;

	// SRV番号取得
	inline int GetSRVNumber() const { return m_SRVNumber; }

private:
	int m_SRVNumber = 0;

};