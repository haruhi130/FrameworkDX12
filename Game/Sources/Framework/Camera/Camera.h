#pragma once

class Camera
{
public:
	Camera() { SetProjectionMatrix(60.0f); }
	~Camera() {}

	void Set()const;

	virtual void SetCameraMatrix(const Math::Matrix& mCam)
	{ m_mView = mCam; }

	// Ë‰es—ñ‚Ìİ’è
	void SetProjectionMatrix(float fov, float minRange = 0.01f, float maxRange = 1000.0f, float aspectRatio = 16.0f / 9.0f);
	void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& rProj);

private:
	Math::Matrix m_mView;

	// Ë‰es—ñ
	Math::Matrix m_mProj;

};