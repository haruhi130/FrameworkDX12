#include "ShaderManager.h"

bool ShaderManager::Init()
{
	// 各シェーダーの初期化
	if (!m_postProcessShader.Init()) { return false; }
	if (!m_modelShader.Init()) { return false; }
	if (!m_spriteShader.Init()) { return false; }

	// 影描画範囲設定
	SetDirLightShadowArea({ 50.0f,50.0f }, 50.0f);

	WriteCBShadowArea(m_shadowProj, m_dirLightHeight);

	return true;
}

void ShaderManager::PreUpdate()
{
	m_pointLights.clear();
	m_spotLights.clear();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// カメラ情報
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
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
		->BindAndAttachData(4, m_cbCamera);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ライト情報
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void ShaderManager::SetIsUseLight(bool isUseLight)
{
	m_cbLight.IsUseLight = isUseLight;
	WriteCBLight();
}

void ShaderManager::SetDirectionalLight(const Math::Vector3& dir, const Math::Vector3& color)
{
	m_cbLight.DirectionalLightDir = dir;
	m_cbLight.DirectionalLightDir.Normalize();
	m_cbLight.DirectionalLightColor = color;
}

void ShaderManager::SetAmbientLight(float pow)
{
	m_cbLight.AmbientLightPower = pow;
}

void ShaderManager::SetDirLightShadowArea(const Math::Vector2& area, float height)
{
	m_shadowProj = DirectX::XMMatrixOrthographicLH(area.x, area.x, 0.0f, height * 2.0f);

	m_dirLightHeight = height;
}

void ShaderManager::WriteCBLight() const
{
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(5, m_cbLight);
}

void ShaderManager::WriteCBShadowArea(const Math::Matrix& proj, float dirLightHeight)
{
	Math::Vector3 lightDir = m_cbLight.DirectionalLightDir;
	Math::Vector3 lightPos = m_cbCamera.CamPos;
	Math::Vector3 up = (lightDir == Math::Vector3::Up) ? Math::Vector3::Right : Math::Vector3::Up;

	Math::Matrix shadowVP = DirectX::XMMatrixLookAtLH(lightPos - lightDir * dirLightHeight, lightPos, up);

	shadowVP *= proj;
	m_cbLight.DirLight_mVP = shadowVP;
}

void ShaderManager::WriteLightParams()
{
	if (m_pointLights.size())
	{
		WritePointLight(m_pointLights);
	}

	if (m_spotLights.size())
	{
		WriteSpotLight(m_spotLights);
	}

	WriteCBShadowArea(m_shadowProj, m_dirLightHeight);
	WriteCBLight();
}

void ShaderManager::WritePointLight(const std::list<ConstantBufferData::PointLight>& pointLights)
{
	m_cbLight.PointLight_Num = int(pointLights.size());

	UINT index = 0;
	for (auto& pointLight : pointLights)
	{
		m_cbLight.PointLights[index] = pointLight;
		++index;
	}

	WriteCBLight();
}

void ShaderManager::WriteSpotLight(const std::list<ConstantBufferData::SpotLight>& spotLights)
{
	m_cbLight.SpotLight_Num = int(spotLights.size());

	UINT index = 0;
	for (auto& spotLight : spotLights)
	{
		m_cbLight.SpotLights[index] = spotLight;
		++index;
	}

	WriteCBLight();
}

void ShaderManager::AddPointLight(const Math::Vector3& color, float radius, const Math::Vector3& pos, bool isBright)
{
	m_pointLights.push_back(ConstantBufferData::PointLight(color, radius, pos, isBright));
}

void ShaderManager::AddPointLight(const ConstantBufferData::PointLight& pointLight)
{
	m_pointLights.push_back(pointLight);
}

void ShaderManager::AddSpotLight(const Math::Vector3& color, float range, const Math::Vector3& pos, float angle, const Math::Vector3& dir, bool isEnable)
{
	m_spotLights.push_back(ConstantBufferData::SpotLight(isEnable, color, range, pos, DirectX::XMConvertToRadians(angle), dir));
}

void ShaderManager::AddSpotLight(const ConstantBufferData::SpotLight& spotLight)
{
	m_spotLights.push_back(spotLight);
}