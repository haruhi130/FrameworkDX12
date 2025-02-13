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

	UpdateCollision();

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
	m_local = { 0.5f,1.5f,-2.3f };

	m_localPos = Math::Matrix::CreateTranslation(m_local);

	m_mousePos = { window_width / 2,window_height / 2 };

	EffekseerManager::GetInstance().SetCamera(m_spCamera);
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

	m_vec.x = std::clamp(m_vec.x, -60.0f, 60.0f);
}

void GameCamera::UpdateCollision()
{
	const std::shared_ptr<const BaseObject> target = m_wpTarget.lock();
	if (!target)return;

	Collider::RayInfo rayInfo;
	rayInfo.m_pos = target->GetPos();
	rayInfo.m_pos.y += 1.0f;

	rayInfo.m_dir = GetPos() - rayInfo.m_pos;
	rayInfo.m_dir.Normalize();

	rayInfo.m_range = (GetPos() - rayInfo.m_pos).Length();
	rayInfo.m_type = Collider::Type::Ground | Collider::Type::Bump;

	for (std::weak_ptr<BaseObject> wpObj : m_wpHitObjList)
	{
		std::shared_ptr<BaseObject> spObj = wpObj.lock();
		if (spObj)
		{
			std::list<Collider::CollisionResult> retRayList;
			spObj->Intersects(rayInfo, &retRayList);

			float lap = 0.0f;
			Math::Vector3 hitPos = Math::Vector3::Zero;
			bool isHit = false;

			for (auto& ret : retRayList)
			{
				if (lap < ret.m_overlapDistance)
				{
					lap = ret.m_overlapDistance;
					hitPos += ret.m_hitPos + (Math::Vector3(0,0.1f,0.1f) * ret.m_overlapDistance);
					isHit = true;
				}
			}

			if (isHit)
			{
				SetPos(hitPos);
			}
		}
	}
}
