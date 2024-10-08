#pragma once

enum class DepthStencilFormat
{
	DepthLowQuality = DXGI_FORMAT_R16_TYPELESS,
	DepthHighQuality = DXGI_FORMAT_R32_TYPELESS,
	DepthHighQualityAndStencil = DXGI_FORMAT_R24G8_TYPELESS,
};

class DepthStencil : public Buffer
{
public:
	~DepthStencil()override{}

	// 深度テクスチャ作成
	bool Create(const Math::Vector2& resolution, DepthStencilFormat format = DepthStencilFormat::DepthHighQuality);

	// 深度バッファデータ初期化
	void ClearBuffer() const;

	// DSV番号取得
	inline UINT GetDSVNumber() const
	{ return m_DSVNumber; }

private:
	UINT m_DSVNumber = 0;
};