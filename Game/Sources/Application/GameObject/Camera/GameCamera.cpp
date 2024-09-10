#include "GameCamera.h"

void GameCamera::PostUpdate()
{
	if (!m_spCamera) return;

	Math::Matrix target = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		target = Math::Matrix::CreateTranslation(m_wpTarget.lock()->GetPos());
	}

	UpdateRotateByMouse();

	m_mRot = GetRotationMatrix();

	m_localPos = Math::Matrix::CreateTranslation(m_local);

	m_mWorld = m_localPos * m_mRot * target;

	m_spCamera->SetCameraMatrix(m_mWorld);
}

void GameCamera::PreDraw()
{
	m_spCamera->Set();
}

void GameCamera::SetTarget(const std::shared_ptr<BaseObject>& target)
{
	if (!target) return;
	m_wpTarget = target;
}

void GameCamera::Init()
{
	if (!m_spCamera)
	{
		m_spCamera = std::make_shared<Camera>();
	}

	m_mWorld = Math::Matrix::Identity;

	m_vec = {};
	m_local = { 0,3,-5 };

	m_localPos = Math::Matrix::CreateTranslation(m_local);

	m_mousePos = { 640,360 };
}

void GameCamera::UpdateRotateByMouse()
{
	POINT nowPos = {};
	GetCursorPos(&nowPos);

	POINT move = {};
	move.x = nowPos.x - m_mousePos.x;
	move.y = nowPos.y - m_mousePos.y;

	SetCursorPos(m_mousePos.x, m_mousePos.y);

	m_vec.x += move.y * 0.15f;
	m_vec.y += move.x * 0.15f;

	m_vec.x = std::clamp(m_vec.x, -FLT_MAX, FLT_MAX);
}
