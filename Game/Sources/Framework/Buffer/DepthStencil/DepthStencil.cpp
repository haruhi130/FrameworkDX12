#include "DepthStencil.h"

bool DepthStencil::Create(const Math::Vector2& resolution, DepthStencilFormat format)
{
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Width = static_cast<UINT>(resolution.x);
	resDesc.Height = static_cast<UINT>(resolution.y);
	resDesc.DepthOrArraySize = 1;
	resDesc.Format = static_cast<DXGI_FORMAT>(format);
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	// �f�v�X�o�b�t�@�̃t�H�[�}�b�g�ƍő�[�x�l
	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.DepthStencil.Depth = 1.0f;

	switch (format)
	{
	case DepthStencilFormat::DepthLowQuality:
		clearValue.Format = DXGI_FORMAT_D16_UNORM;
		break;
	case DepthStencilFormat::DepthHighQuality:
		clearValue.Format = DXGI_FORMAT_D32_FLOAT;
		break;
	case DepthStencilFormat::DepthHighQualityAndStencil:
		clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		break;
	default:
		break;
	}

	// �f�v�X�o�b�t�@�쐬
	auto result = GraphicsDevice::GetInstance().GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue, IID_PPV_ARGS(m_cpBuffer.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		assert(0 && "�f�v�X�X�e���V���o�b�t�@�쐬���s");
		return false;
	}

	// DSV�쐬
	m_DSVNumber = GraphicsDevice::GetInstance().GetDSVHeap()->CreateDSV(m_cpBuffer.Get(), clearValue.Format);

	return true;
}

void DepthStencil::ClearBuffer() const
{
	GraphicsDevice::GetInstance().GetCmdList()->ClearDepthStencilView(
		GraphicsDevice::GetInstance().GetDSVHeap()->GetCPUHandle(m_DSVNumber),
		D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}
