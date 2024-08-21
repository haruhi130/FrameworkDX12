#include "Camera.h"

void Camera::Set()
{
	m_mView = Math::Matrix::CreateTranslation(m_camPos);

	ConstantBufferData::Camera cbCamera;
	cbCamera.mView = m_mView;
	cbCamera.mProj = m_mProj;
	cbCamera.mProjInv = m_mProj.Invert();
	cbCamera.CamPos = m_camPos;

	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(0, cbCamera);
}

// 射影行列の設定：各種パラメータから射影行列を生成して保持する
void Camera::SetProjectionMatrix(float fov, float minRange, float maxRange, float aspectRatio)
{
	float aspect = aspectRatio;

	// アスペクト比が不正だった場合
	if (aspect <= 0)
	{
		return;
	}

	m_mProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspect, minRange, maxRange);

	SetProjectionMatrix(m_mProj);
}

// 射影行列の設定：既存の射影行列をコピーする
void Camera::SetProjectionMatrix(const DirectX::SimpleMath::Matrix& rProj)
{
	m_mProj = rProj;
}
