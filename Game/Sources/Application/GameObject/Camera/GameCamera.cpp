#include "GameCamera.h"

void GameCamera::PostUpdate()
{
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_vec.y += 0.1f;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_vec.y -= 0.1f;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_vec.x += 0.1f;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_vec.x -= 0.1f;
	}
	if (GetAsyncKeyState('X') & 0x8000)
	{
		m_vec.z += 0.1f;
	}
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		m_vec.z -= 0.1f;
	}
	m_spCamera->SetCameraMatrix
	(Math::Matrix::CreateTranslation(m_vec));
}

void GameCamera::PreDraw()
{
	m_spCamera->Set();
}

void GameCamera::SetTarget(const std::shared_ptr<BaseObject>& target)
{
}

void GameCamera::Init()
{
	if (!m_spCamera)
	{
		m_spCamera = std::make_shared<Camera>();
	}

	m_mWorld = Math::Matrix::Identity;

	m_vec = {0,0,10};

}