#include "RenderTargetChange.h"

bool RenderTargetChange::CreateRenderTarget()
{
	// �e�N�X�`�����烊�\�[�X�쐬
	if (!m_spRTTexture)
	{
		m_spRTTexture = std::make_shared<Texture>();
	}
	if (!m_spRTTexture->CreateResource())
	{
		return false;
	}

	// RTV�쐬
	m_rtvNum = GraphicsDevice::GetInstance().GetRTVHeap()->CreateRTV(m_spRTTexture->GetBuffer().Get());

	// SRV�쐬
	m_renderTargetSRVNumber = GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->CreateSRV(m_spRTTexture->GetBuffer().Get());

	return true;
}

void RenderTargetChange::ChangeRenderTarget() const
{
	// ���\�[�X�o���A�؂�ւ�
	GraphicsDevice::GetInstance().SetResourceBarrier(m_spRTTexture->GetBuffer().Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// RTV�n���h���擾
	auto rtvH = GraphicsDevice::GetInstance().GetRTVHeap()->GetCPUHandle(m_rtvNum);
	
	// DSV�n���h���擾
	auto dsvH = GraphicsDevice::GetInstance().GetDSVHeap()->GetCPUHandle(
		GraphicsDevice::GetInstance().GetDepthStencil()->GetDSVNumber());

	// �����_�[�^�[�Q�b�g�؂�ւ�
	GraphicsDevice::GetInstance().GetCmdList()->OMSetRenderTargets(
		1, &rtvH, false, &dsvH);

	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	GraphicsDevice::GetInstance().GetCmdList()->ClearRenderTargetView(
		rtvH, clearColor, 0, nullptr);

	// �ʏ�[�x�o�b�t�@�N���A
	GraphicsDevice::GetInstance().GetDepthStencil()->ClearBuffer();
}

void RenderTargetChange::UndoRenderTarget() const
{
	// ���\�[�X�o���A�؂�ւ�
	GraphicsDevice::GetInstance().SetResourceBarrier(m_spRTTexture->GetBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

bool RenderTargetChange::CreateRTTexture()
{
	// �����_�[�^�[�Q�b�g�p�|���쐬
	if (!m_spRTTexture)
	{
		m_spRTTexture = std::make_shared<Texture>();
	}
	if (!m_spRTTexture->CreateRenderTarget())
	{
		return false;
	}

	return true;
}

bool RenderTargetChange::CreateDepthTexture()
{
	// �[�x�e�N�X�`���쐬
	if (!m_spDepthTexture)
	{
		m_spDepthTexture = std::make_shared<Texture>();
	}
	if (!m_spDepthTexture->CreateDepthSRV())
	{
		return false;
	}

	return true;
}

void RenderTargetChange::Draw() const
{
	m_spRTTexture->SetToDevice();

	GraphicsDevice::GetInstance().GetCmdList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
