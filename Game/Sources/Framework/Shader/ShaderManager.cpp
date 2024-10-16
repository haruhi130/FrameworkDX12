#include "ShaderManager.h"

void ShaderManager::Init()
{
	// 各シェーダーのInitを呼び出し
	m_modelShader.Init();
	m_postProcessShader.Init();
	m_spriteShader.Init();
}

void ShaderManager::SetCamera(const Math::Matrix& view, const Math::Matrix& proj)
{
	m_cbCamera.mView = view.Invert();
	m_cbCamera.mProj = proj;
	m_cbCamera.mProjInv = proj.Invert();
	m_cbCamera.CamPos = view.Translation();
}

void ShaderManager::WriteCBCamera() const
{
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(0, m_cbCamera);
}

void ShaderManager::SetIsUseLight(bool isUseLight)
{
	m_cbLight.IsUseLight = isUseLight;

	WriteCBLight();
}

void ShaderManager::SetDirectionalLight(const Math::Vector3& dir, const Math::Vector3& color)
{
	m_cbLight.DirectionalLightDir = dir;
	m_cbLight.DirectionalLightColor = color;
}

void ShaderManager::SetAmbientLight(float pow)
{
	m_cbLight.AmbientLightPower = pow;
}

void ShaderManager::WriteCBLight() const
{
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(5, m_cbLight);
}

