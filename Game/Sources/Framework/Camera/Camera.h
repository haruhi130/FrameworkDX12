#pragma once

class Camera
{
public:
	Camera() { SetProjectionMatrix(60.0f); }
	~Camera(){}

	void Set();

	virtual void SetCameraMatrix(const Math::Matrix& mCam)
	{ m_mView = mCam; }

	// �ˉe�s��̐ݒ�
	void SetProjectionMatrix(float fov, float minRange = 0.01f,float maxRange = 1000.0f, float aspectRatio = 1280.0f/720.0f);
	void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& rProj);
	
private:
	Math::Matrix m_mView;

	// �ˉe�s��
	Math::Matrix m_mProj;

};