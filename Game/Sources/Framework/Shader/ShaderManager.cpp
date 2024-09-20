#include "ShaderManager.h"

void ShaderManager::Init()
{
	m_modelShader.Init();
}

void ShaderManager::SetCamera(const Math::Matrix& view, const Math::Matrix& proj)
{
	m_cbCamera.mView = view.Invert();
	m_cbCamera.mProj = proj;
	m_cbCamera.mProjInv = proj.Invert();
	m_cbCamera.CamPos = view.Translation();
}

void ShaderManager::WriteCamera() const
{
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(0, m_cbCamera);
}
