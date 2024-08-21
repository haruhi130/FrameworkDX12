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

// �ˉe�s��̐ݒ�F�e��p�����[�^����ˉe�s��𐶐����ĕێ�����
void Camera::SetProjectionMatrix(float fov, float minRange, float maxRange, float aspectRatio)
{
	float aspect = aspectRatio;

	// �A�X�y�N�g�䂪�s���������ꍇ
	if (aspect <= 0)
	{
		return;
	}

	m_mProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspect, minRange, maxRange);

	SetProjectionMatrix(m_mProj);
}

// �ˉe�s��̐ݒ�F�����̎ˉe�s����R�s�[����
void Camera::SetProjectionMatrix(const DirectX::SimpleMath::Matrix& rProj)
{
	m_mProj = rProj;
}
