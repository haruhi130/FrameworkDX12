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

	// �[�x�e�N�X�`���쐬
	bool Create(const Math::Vector2& resolution, DepthStencilFormat format = DepthStencilFormat::DepthHighQuality);

	// �[�x�o�b�t�@�f�[�^������
	void ClearBuffer() const;

	// DSV�ԍ��擾
	inline UINT GetDSVNumber() const
	{ return m_DSVNumber; }

private:
	UINT m_DSVNumber = 0;
};