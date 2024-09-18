#include "Mouse.h"
#include "../Camera/GameCamera.h"

#include "Application/Application.h"


void Mouse::Update()
{
	m_gravity += 0.005f;
	m_mWorld._42 -= m_gravity;

	m_vec = m_mWorld.Translation();
	// �X�e�[�g�X�V
	if (m_currentAction)
	{
		m_currentAction->Update(*this);
	}

	UpdateMatrix();
	UpdateCollision();
}

void Mouse::PostUpdate()
{
	// �A�j���[�V��������
	m_spAnimator->ProgressTime(m_spModel->WorkNodes(),animeTime);
	m_spModel->CalcNodeMatrices();
}

void Mouse::Draw()
{
	// ���f���`��
	Application::GetInstance().GetShader().DrawModel(*m_spModel, m_mWorld);
}

void Mouse::Init()
{
	// ���f���ǂݍ���
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData("Assets/Models/Mouse/Mouse.gltf");
	}

	// �����v�Z
	Math::Matrix mScale = Math::Matrix::CreateScale(0.5f);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(0, 0, -20);
	m_mWorld = mScale * mTrans;

	// �A�j���[�V�����ݒ�
	if (!m_spAnimator)
	{
		m_spAnimator = std::make_shared<Animator>();
	}

	// �X�e�[�g�ݒ�u�ҋ@�v
	ChangeActionState(std::make_shared<ActionIdle>());

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Mouse", m_spModel, Collider::Type::Sight);
}

void Mouse::UpdateMatrix()
{
	Math::Matrix mScale = Math::Matrix::CreateScale(0.5f);
	Math::Matrix mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rot.y));
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(m_vec);
	m_mWorld = mScale * mRot * mTrans;
}

void Mouse::UpdateRotate(Math::Vector3& moveVec)
{
	if (moveVec.LengthSquared() == 0.0f){ return; }

	Math::Vector3 targetDir = moveVec;
	targetDir.Normalize();
	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	Math::Vector3 nowDir = GetMatrix().Backward();
	nowDir.Normalize();
	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	float betweenAng = targetAng - nowAng;
	if (betweenAng > 180)
	{
		betweenAng -= 360;
	}
	else if (betweenAng < -180)
	{
		betweenAng += 360;
	}

	float rotAng = std::clamp(betweenAng, -20.0f, 20.0f);

	m_rot.y += rotAng;
}

void Mouse::UpdateCollision()
{
	// Ray : Ground
	{
		Collider::RayInfo rayInfo;
		rayInfo.m_pos = GetPos();
		rayInfo.m_pos.y += 0.3f;

		rayInfo.m_dir = Math::Vector3::Down;
		rayInfo.m_range = m_gravity + 0.3f;

		rayInfo.m_type = Collider::Type::Ground;

		for (std::weak_ptr<BaseObject> wpObj : m_wpHitObjList)
		{
			std::shared_ptr<BaseObject> spObj = wpObj.lock();
			if (spObj)
			{
				std::list<Collider::CollisionResult> retRayList;
				spObj->Intersects(rayInfo, &retRayList);

				float lap = 0.0f;
				Math::Vector3 hit = Math::Vector3::Zero;
				bool isHit = false;

				for (auto& ret : retRayList)
				{
					if (lap < ret.m_overlapDistance)
					{
						lap = ret.m_overlapDistance;
						hit = ret.m_hitPos;
						isHit = true;
					}
				}

				if (isHit)
				{
					SetPos(hit);
					m_gravity = 0.0f;
					m_isGround = true;
				}
			}
		}
	}

	// Sphere : Bump
	{
		Collider::SphereInfo sphereInfo;
		sphereInfo.m_sphere.Center = GetPos() + Math::Vector3(0, 1.4f, 0);
		sphereInfo.m_sphere.Radius = 1.2f;
		sphereInfo.m_type = Collider::Type::Bump;

		for (std::weak_ptr<BaseObject> wpObj : m_wpHitObjList)
		{
			std::shared_ptr<BaseObject> spObj = wpObj.lock();
			if (spObj)
			{
				std::list<Collider::CollisionResult> retBumpList;
				spObj->Intersects(sphereInfo, &retBumpList);

				for (auto& ret : retBumpList)
				{
					Math::Vector3 newPos = GetPos() + (ret.m_hitDir * ret.m_overlapDistance);
					SetPos(newPos);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
// �X�e�[�g�Ǘ�
// �X�e�[�g�ύX
void Mouse::ChangeActionState(std::shared_ptr<ActionStateBase> nextState)
{
	if (m_currentAction)m_currentAction->Exit(*this);
	m_currentAction = nextState;
	m_currentAction->Enter(*this);
}

//////////////////////////////////////////////////////////////////////////////
// �ҋ@���
void Mouse::ActionIdle::Enter(Mouse& owner)
{
	owner.m_spAnimator->SetAnimation(owner.m_spModel->GetModelData()->GetAnimation("Idle"));
}

void Mouse::ActionIdle::Update(Mouse& owner)
{
	Math::Vector3 vec = Math::Vector3::Zero;
	if (GetAsyncKeyState('W')) { vec.z = 1.0f; }
	if (GetAsyncKeyState('S')) { vec.z = -1.0f; }
	if (GetAsyncKeyState('A')) { vec.x = -1.0f; }
	if (GetAsyncKeyState('D')) { vec.x = 1.0f; }

	if (vec.LengthSquared() > 0)
	{
		owner.ChangeActionState(std::make_shared<ActionWalk>());
	}
}

void Mouse::ActionIdle::Exit(Mouse& owner)
{
}

//////////////////////////////////////////////////////////////////////////////
// �������
void Mouse::ActionWalk::Enter(Mouse& owner)
{
	owner.m_spAnimator->SetAnimation(owner.m_spModel->GetModelData()->GetAnimation("Walk"));
}

void Mouse::ActionWalk::Update(Mouse& owner)
{
	float spd = 0.1f;

	Math::Vector3 vec = Math::Vector3::Zero;
	if (GetAsyncKeyState('W')) { vec.z = 1.0f; }
	if (GetAsyncKeyState('S')) { vec.z = -1.0f; }
	if (GetAsyncKeyState('A')) { vec.x = -1.0f; }
	if (GetAsyncKeyState('D')) { vec.x = 1.0f; }

	if (vec.LengthSquared() == 0)
	{
		owner.ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}

	std::shared_ptr<GameCamera> spCamera = owner.m_wpCamera.lock();
	if (spCamera)
	{
		vec = vec.TransformNormal(vec, spCamera->GetRotationYMatrix());
	}

	vec.Normalize();
	vec *= spd;

	owner.m_vec.x += vec.x;
	owner.m_vec.z += vec.z;

	owner.UpdateRotate(vec);
}

void Mouse::ActionWalk::Exit(Mouse& owner)
{
}
