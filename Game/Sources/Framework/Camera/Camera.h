#pragma once

class Camera
{
public:
	Camera() { SetProjectionMatrix(60.0f); }
	~Camera() {}

	void Set() const;

	inline void SetCameraMatrix(const Math::Matrix& mCam)
	{ m_mView = mCam; }

	// �ˉe�s��̐ݒ�
	void SetProjectionMatrix(float fov, float minRange = 0.01f, float maxRange = 500.0f, float aspectRatio = 16.0f / 9.0f);
	void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& rProj);

	// �N���C�A���g���W����3D���[���h���W�����߂郌�C���𐶐�
	void GenerateRayInfoFromClient(const POINT& clientPos, Math::Vector3& rayPos, Math::Vector3& rayDir, float& rayRange) const;

	inline const Math::Matrix& GetCameraMatrix() const 
	{ return m_mView; }

	inline const Math::Matrix& GetProjMatrix() const
	{ return m_mProj; }

private:
	Math::Matrix m_mView;

	// �ˉe�s��
	Math::Matrix m_mProj;

};