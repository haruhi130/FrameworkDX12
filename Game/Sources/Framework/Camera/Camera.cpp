#include "Camera.h"

void Camera::SetToShader() const
{
	ShaderManager::GetInstance().SetCamera(m_mView,m_mProj);
}

void Camera::SetProjMatrix(float fov, float minRange, float maxRange, float aspectRatio)
{
	float aspect = aspectRatio;

	// アスペクト比が不正だった場合
	if (aspect <= 0)
	{
		// デフォルトのアスペクト比を使用
		aspect = 16.0f / 9.0f;
	}
	
	m_mProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspect, minRange, maxRange);

	SetProjMatrix(m_mProj);
}

void Camera::SetProjMatrix(const DirectX::SimpleMath::Matrix& rProj)
{
	m_mProj = rProj;
}

void Camera::GenerateRayInfoFromClient(const POINT& clientPos, Math::Vector3& rayPos, Math::Vector3& rayDir, float& rayRange) const
{
	// レイ判定の最遠座標
	Math::Vector3 farPos;

	// 2D座標を3D座標に変換しレイ情報を求める
	GraphicsDevice::GetInstance().ClientToWorld(clientPos, 0.0f, rayPos, m_mView, m_mProj);
	GraphicsDevice::GetInstance().ClientToWorld(clientPos, 1.0f, farPos, m_mView, m_mProj);

	rayDir = farPos - rayPos;
	rayRange = rayDir.Length();
	rayDir.Normalize();
}
