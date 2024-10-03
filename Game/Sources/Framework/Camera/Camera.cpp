#include "Camera.h"

void Camera::Set() const
{
	ShaderManager::GetInstance().SetCamera(m_mView,m_mProj);
}

// 射影行列の設定：各種パラメータから射影行列を生成して保持する
void Camera::SetProjectionMatrix(float fov, float minRange, float maxRange, float aspectRatio)
{
	float aspect = aspectRatio;

	// アスペクト比が不正だった場合
	if (aspect <= 0)
	{
		// デフォルトのアスペクト比を使用
		aspect = 16.0f / 9.0f;
	}
	
	m_mProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspect, minRange, maxRange);

	SetProjectionMatrix(m_mProj);
}

// 射影行列の設定：既存の射影行列をコピーする
void Camera::SetProjectionMatrix(const DirectX::SimpleMath::Matrix& rProj)
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

	// レイ方向
	rayDir = farPos - rayPos;

	// レイ判定距離
	rayRange = rayDir.Length();

	// 正規化
	rayDir.Normalize();
}
