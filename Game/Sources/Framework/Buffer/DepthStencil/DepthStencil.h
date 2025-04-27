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
	/// �[�x�e�N�X�`���쐬
	/// </summary>
	/// <param name="resolution">�𑜓x</param>
	/// <param name="format">�[�x�e�N�X�`���t�H�[�}�b�g</param>
	/// <returns></returns>
	bool Create(const Math::Vector2& resolution, DepthStencilFormat format = DepthStencilFormat::DepthHighQuality);

	/// <summary>
	/// �[�x�o�b�t�@�ɏ������܂�Ă���f�[�^������
	/// </summary>
	void ClearBuffer() const;

	/// <summary>
	/// DSV�ԍ��擾
	/// </summary>
	/// <returns>DSV�ԍ�</returns>
	inline UINT GetDSVNumber() const
	{ return m_DSVNumber; }

private:
	UINT m_DSVNumber = 0;
};