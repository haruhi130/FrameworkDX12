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

void Camera::GenerateRayInfoFromClient(const POINT& clientPos, Math::Vector3& rayPos, Math::Vector3& rayDir, float& rayRange) const
{
	// ���C����̍ŉ����W
	Math::Vector3 farPos;

	// 2D���W��3D���W�ɕϊ������C�������߂�
	GraphicsDevice::GetInstance().ClientToWorld(clientPos, 0.0f, rayPos, m_mView, m_mProj);
	GraphicsDevice::GetInstance().ClientToWorld(clientPos, 1.0f, farPos, m_mView, m_mProj);

	// ���C����
	rayDir = farPos - rayPos;

	// ���C���苗��
	rayRange = rayDir.Length();

	// ���K��
	rayDir.Normalize();
}
