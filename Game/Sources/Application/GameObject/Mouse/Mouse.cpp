#include "Mouse.h"
#include "../Camera/GameCamera.h"
#include "../../Scene/SceneManager.h"

void Mouse::Update()
{
	if (!m_spModel) { return; }

	// 重力処理
	m_pos.y -= m_gravity;
	m_gravity += 0.005f;

	// ステート更新
	if (m_currentAction)
	{
		m_currentAction->Update(*this);
	}

	// 行列更新
	UpdateMatrix();
	// 当たり判定更新
	UpdateCollision();
}

void Mouse::PostUpdate()
{
	if (!m_spModel) { return; }
	if (!m_spAnimator) { return; }

	// アニメーション処理
	m_spAnimator->ProgressTime(m_spModel->WorkNodes());
	m_spModel->CalcNodeMatrices();
}

void Mouse::Draw()
{
	if (!m_spModel) { return; }

	// モデル描画
	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void Mouse::Init()
{
	// モデル読み込み
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.
			GetData("Assets/Models/Mouse/Mouse.gltf"));
	}

	m_speed = 5.0f;

	// 初期計算
	Math::Matrix mScale = Math::Matrix::CreateScale(0.5f);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(0, 0, -20);
	m_mWorld = mScale * mTrans;

	// アニメーション設定
	if (!m_spAnimator)
	{
		m_spAnimator = std::make_shared<Animator>();
	}

	// ステート設定「待機」
	ChangeActionState(std::make_shared<ActionIdle>());

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Mouse", m_spModel, Collider::Type::Sight | Collider::Type::Bump);
}

void Mouse::UpdateMatrix()
{
	Math::Matrix mScale = Math::Matrix::CreateScale(0.5f);
	Math::Matrix mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rot.y));
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = mScale * mRot * mTrans;
}

void Mouse::UpdateRotate(Math::Vector3& moveVec)
{
	if (moveVec.LengthSquared() == 0.0f) { return; }

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

	float rotAng = std::clamp(betweenAng, -36.0f, 36.0f);

	m_rot.y += rotAng;
}

void Mouse::UpdateCollision()
{
	// Ray : Ground
	{
		Collider::RayInfo rayInfo;
		rayInfo.m_pos = m_pos;
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
				Math::Vector3 hitPos = Math::Vector3::Zero;
				bool isHit = false;

				for (auto& ret : retRayList)
				{
					if (lap < ret.m_overlapDistance)
					{
						lap = ret.m_overlapDistance;
						hitPos = ret.m_hitPos;
						isHit = true;
					}
				}

				if (isHit)
				{
					m_pos = hitPos;
					SetPos(m_pos);
					m_gravity = 0.0f;
					m_isGround = true;
				}
			}
		}
	}

	// Sphere : Bump
	{
		Collider::SphereInfo sphereInfo;
		sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 0.8f, 0);
		sphereInfo.m_sphere.Radius = 0.6f;
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
					Math::Vector3 newPos = m_pos + (ret.m_hitDir * ret.m_overlapDistance);
					m_pos = newPos;
					SetPos(m_pos);
				}
			}
		}
	}

	// Ray : Event
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			// マウス情報
			POINT mousePos = {};
			GetCursorPos(&mousePos);

			// カメラ情報
			const std::shared_ptr<GameCamera> spCamera = m_wpCamera.lock();
			if (spCamera)
			{
				Math::Vector3 rayDir = Math::Vector3::Zero;
				float range = 1.0f;

				Math::Vector3 cameraPos = spCamera->GetPos();

				// 3D座標に変換
				spCamera->WorkCamera()->GenerateRayInfoFromClient(
				mousePos, cameraPos, rayDir, range);

				for (std::weak_ptr<BaseObject> wpObj : m_wpHitObjList)
				{
					std::shared_ptr<BaseObject> spObj = wpObj.lock();
					if (spObj)
					{
						Math::Vector3 endRayPos = cameraPos + rayDir * range;

						Collider::RayInfo rayInfo
						(
							Collider::Type::Event,
							cameraPos,
							endRayPos);

						std::list<Collider::CollisionResult> retRayList;
						spObj->Intersects(rayInfo, &retRayList);

						for (auto& ret : retRayList)
						{
							m_pos = ret.m_hitPos;
							SetPos(m_pos);
						}
					}
				}
			}
		}
	}

	// Sphere : Goal
	{
		Collider::SphereInfo sphereInfo;
		sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 0.8f, 0);
		sphereInfo.m_sphere.Radius = 0.6f;
		sphereInfo.m_type = Collider::Type::Goal;

		for (std::weak_ptr<BaseObject> wpObj : m_wpHitObjList)
		{
			std::shared_ptr<BaseObject> spObj = wpObj.lock();
			if (spObj)
			{
				std::list<Collider::CollisionResult> retBumpList;
				spObj->Intersects(sphereInfo, &retBumpList);

				for (auto& ret : retBumpList)
				{
					SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Result);
				}
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
	if (GetAsyncKeyState('W')) { vec = Math::Vector3::Backward; }
	if (GetAsyncKeyState('S')) { vec = Math::Vector3::Forward; }
	if (GetAsyncKeyState('A')) { vec = Math::Vector3::Left; }
	if (GetAsyncKeyState('D')) { vec = Math::Vector3::Right; }

	if (vec.LengthSquared() > 0.0f)
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
	auto time = ServiceLocator::Get<Time_VRR>();
	float speed = owner.m_speed * time->DeltaTime();

	Math::Vector3 move = Math::Vector3::Zero;
	if (GetAsyncKeyState('W'))
	{
		Math::Vector3 vec = Math::Vector3::Backward;
		move += vec;
	}
	if (GetAsyncKeyState('S'))
	{
		Math::Vector3 vec = Math::Vector3::Forward;
		move += vec;
	}
	if (GetAsyncKeyState('A'))
	{
		Math::Vector3 vec = Math::Vector3::Left;
		move += vec;
	}
	if (GetAsyncKeyState('D'))
	{
		Math::Vector3 vec = Math::Vector3::Right;
		move += vec;
	}

	if (move.LengthSquared() == 0)
	{
		owner.ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}

	std::shared_ptr<GameCamera> spCamera = owner.m_wpCamera.lock();
	if (spCamera)
	{
		move = move.TransformNormal(move, spCamera->GetRotationYMatrix());
	}

	move.Normalize();
	move *= speed;

	owner.m_pos.x += move.x;
	owner.m_pos.z += move.z;

	owner.UpdateRotate(move);
}

void Mouse::ActionWalk::Exit(Mouse& owner)
{
}
