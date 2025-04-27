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

	/// <summary>
	/// 深度テクスチャ作成
	/// </summary>
	/// <param name="resolution">解像度</param>
	/// <param name="format">深度テクスチャフォーマット</param>
	/// <returns></returns>
	bool Create(const Math::Vector2& resolution, DepthStencilFormat format = DepthStencilFormat::DepthHighQuality);

	/// <summary>
	/// 深度バッファに書き込まれているデータ初期化
	/// </summary>
	void ClearBuffer() const;

	/// <summary>
	/// DSV番号取得
	/// </summary>
	/// <returns>DSV番号</returns>
	inline UINT GetDSVNumber() const
	{ return m_DSVNumber; }

private:
	UINT m_DSVNumber = 0;
};