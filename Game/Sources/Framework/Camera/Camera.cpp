#include "Camera.h"

void Camera::Set() const
{
	ShaderManager::GetInstance().SetCamera(m_mView,m_mProj);
}

// �ˉe�s��̐ݒ�F�e��p�����[�^����ˉe�s��𐶐����ĕێ�����
void Camera::SetProjectionMatrix(float fov, float minRange, float maxRange, float aspectRatio)
{
	float aspect = aspectRatio;

	// �A�X�y�N�g�䂪�s���������ꍇ
	if (aspect <= 0)
	{
		// �f�t�H���g�̃A�X�y�N�g����g�p
		aspect = 16.0f / 9.0f;
	}
	
	m_mProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspect, minRange, maxRange);

	SetProjectionMatrix(m_mProj);
}

// �ˉe�s��̐ݒ�F�����̎ˉe�s����R�s�[����
void Camera::SetProjectionMatrix(const DirectX::SimpleMath::Matrix& rProj)
{
	m_mProj = rProj;
}
