#include "Mouse.h"
#include "../Camera/GameCamera.h"

#include "Application/Application.h"


void Mouse::Update()
{

	m_gravity += 0.01f;
	m_mWorld._42 -= m_gravity;

	m_vec = m_mWorld.Translation();
	// ステート更新
	if (m_currentAction)
	{
		m_currentAction->Update(*this);
	}

	Math::Matrix scale = Math::Matrix::CreateScale(10000.0f);
	Math::Matrix rot = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90))
		* Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180)) * Math::Matrix::CreateRotationY(
		DirectX::XMConvertToRadians(m_rot.y));
	m_mWorld = scale * rot * Math::Matrix::CreateTranslation(m_vec);
	UpdateCollision();

}

void Mouse::PostUpdate()
{
	// アニメーション処理
	m_spAnimator->ProgressTime(m_spModel->WorkNodes(),animeTime);
	m_spModel->CalcNodeMatrices();
}

void Mouse::Draw()
{
	// モデル描画
	Application::GetInstance().GetShader().DrawModel(*m_spModel, m_mWorld);
}

void Mouse::Init()
{
	// モデル読み込み
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData("Assets/Models/Mouse/Mouse.gltf");
	}

	m_gravity = 0.0f;
	m_isGround = false;

	// 初期計算
	Math::Matrix mScale = Math::Matrix::CreateScale(10000.0f);
	Math::Matrix mRot = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90))
		* Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180));
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(0, 30, 0);
	m_mWorld = mScale * mRot * mTrans;

	// アニメーション設定
	if (!m_spAnimator)
	{
		m_spAnimator = std::make_shared<Animator>();
		m_spAnimator->SetAnimation(m_spModel->GetAnimation("Idle"));
	}

	// ステート設定「待機」
	ChangeActionState(std::make_shared<ActionIdle>());
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
	Collider::RayInfo rayInfo;
	rayInfo.m_pos = GetPos();
	rayInfo.m_pos.y += 0.2f;

	rayInfo.m_dir = Math::Vector3::Down;
	rayInfo.m_range = m_gravity + 0.2f;

	rayInfo.m_type = Collider::Type::Bump;

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

//////////////////////////////////////////////////////////////////////////////
// ステート管理
// ステート変更
void Mouse::ChangeActionState(std::shared_ptr<ActionStateBase> nextState)
{
	if (m_currentAction)m_currentAction->Exit(*this);
	m_currentAction = nextState;
	m_currentAction->Enter(*this);
}

//////////////////////////////////////////////////////////////////////////////
// 待機状態
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
// 歩き状態
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
