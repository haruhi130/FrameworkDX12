#pragma once

class Camera
{
public:
	Camera() { SetProjectionMatrix(60.0f); }
	~Camera(){}

	void Set();

	// Ë‰es—ñ‚Ìİ’è
	void SetProjectionMatrix(float fov, float minRange = 0.01f,float maxRange = 1000.0f, float aspectRatio = 1280.0f/720.0f);
	void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& rProj);

	const Math::Vector3& GetCamPos()const { return m_camPos; }
	void SetCamPos(Math::Vector3 camPos) { m_camPos = camPos; }

private:
	Math::Vector3 m_camPos;

	Math::Matrix m_mView;

	// Ë‰es—ñ
	Math::Matrix m_mProj;

};