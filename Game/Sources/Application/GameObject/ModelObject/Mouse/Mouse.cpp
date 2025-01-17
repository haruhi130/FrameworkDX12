#include "Mouse.h"
#include "../../Camera/GameCamera.h"
#include "../../../Scene/SceneManager.h"

void Mouse::Update()
{
	if (!m_spModel) { return; }

	// 重力処理
	m_pos.y -= m_gravity;
	m_gravity += 0.005f;

	--m_coolTime;
	if (m_coolTime <= 0)
	{
		m_coolTime = 0.0f;
	}

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

	if (!m_isMasked)
	{
		// アニメーション処理
		m_spAnimator->ProgressTime(m_spOriginalModel->WorkNodes());
		m_spOriginalModel->CalcNodeMatrices();
	}
}

void Mouse::ImGuiUpdate()
{
	ImGui::Begin(u8"MouseCT");
	ImGui::SetWindowSize(ImVec2(0, 100));
	ImGui::LabelText("MouseCT", "CT : %.2f", m_coolTime);

	ImGui::End();
}

void Mouse::Init()
{
	// モデル読み込み
	if (!m_spOriginalModel)
	{
		m_spOriginalModel = std::make_shared<ModelWork>();
		m_spOriginalModel->SetModelData(Assets::GetInstance().m_modelDatas.
			GetData("Assets/Models/Mouse/Mouse.gltf"));
		m_spModel = m_spOriginalModel;
	}

	m_speed = 3.0f;

	m_pos = { 0,0,10 };

	m_scale = 0.5f;

	// 初期計算
	Math::Matrix mScale = Math::Matrix::CreateScale(m_scale);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = mScale * mTrans;

	// アニメーション設定
	if (!m_spAnimator)
	{
		m_spAnimator = std::make_shared<Animator>();
	}

	// ステート設定「待機」
	ChangeActionState(std::make_shared<ActionIdle>());

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Mouse", m_spOriginalModel, Collider::Type::Sight | Collider::Type::Bump);
}

void Mouse::UpdateMatrix()
{
	Math::Matrix mScale = Math::Matrix::CreateScale(m_scale);
	Math::Matrix mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rot.y));
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = mScale * mRot * mTrans;
}

void Mouse::UpdateRotate(Math::Vector3& moveVec)
{
	if (moveVec.LengthSquared() == 0.0f) { return; }

	auto time = ServiceLocator::Get<Time>();

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
		rayInfo.m_dir.Normalize();
		rayInfo.m_range = m_gravity + 0.3f;

		rayInfo.m_type = Collider::Type::Ground;

		for (std::weak_ptr<ModelObject> wpObj : m_wpObjList)
		{
			std::shared_ptr<ModelObject> spObj = wpObj.lock();
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

		for (std::weak_ptr<ModelObject> wpObj : m_wpObjList)
		{
			std::shared_ptr<ModelObject> spObj = wpObj.lock();
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
	if (InputManager::GetInstance().IsPress("LClick"))
	{
		// マウス情報
		POINT mousePos = {};
		GetCursorPos(&mousePos);

		// カメラ情報
		const std::shared_ptr<GameCamera> spCamera = m_wpCamera.lock();
		if (spCamera)
		{
			// レイの方向
			Math::Vector3 rayDir = Math::Vector3::Zero;
			float range = 0.0f;

			// カメラ座標
			Math::Vector3 cameraPos = spCamera->GetPos();

			// カメラからマウス座標へのレイ情報を作成
			spCamera->WorkCamera()->GenerateRayInfoFromClient(
				mousePos, cameraPos, rayDir, range);

			for (std::weak_ptr<ModelObject> wpObj : m_wpObjList)
			{
				std::shared_ptr<ModelObject> spObj = wpObj.lock();
				if (spObj)
				{
					// レイ情報を当たり判定に設定
					Collider::RayInfo rayInfo;
					rayInfo.m_type = Collider::Type::Event;
					rayInfo.m_pos = cameraPos;
					rayInfo.m_dir = rayDir;
					rayInfo.m_dir.Normalize();
					rayInfo.m_range = 30.0f;

					std::list<Collider::CollisionResult> retRayList;
					spObj->Intersects(rayInfo, &retRayList);
					
					for(auto& ret : retRayList)
					{
						// CT制限
						if (m_coolTime <= 0)
						{
							m_maskedModel = spObj->GetModel();
							m_isMasked = true;
							m_coolTime = 60.0f * 3;
						}
					}
				}
			}
		}
	}

	// Sphere : Goal
	{
		Collider::SphereInfo sphereInfo;
		sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 0.5f, 0);
		sphereInfo.m_sphere.Radius = 3.0f;
		sphereInfo.m_type = Collider::Type::Goal;

		for (std::weak_ptr<ModelObject> wpObj : m_wpObjList)
		{
			std::shared_ptr<ModelObject> spObj = wpObj.lock();
			if (spObj)
			{
				std::list<Collider::CollisionResult> retBumpList;
				if (spObj->Intersects(sphereInfo, &retBumpList))
				{
					for (auto& ret : retBumpList)
					{
						//SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Clear);
					}
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
	if (!owner.m_isMasked)
	{
		owner.m_spAnimator->SetAnimation(owner.m_spOriginalModel->GetModelData()->GetAnimation("Idle"));
		owner.m_spAnimator->ResetAdvanceTime();
	}
}

void Mouse::ActionIdle::Update(Mouse& owner)
{
	Math::Vector3 vec = Math::Vector3::Zero;
	if (InputManager::GetInstance().IsHold("Up")) { vec = Math::Vector3::Backward; }
	if (InputManager::GetInstance().IsHold("Down")) { vec = Math::Vector3::Forward; }
	if (InputManager::GetInstance().IsHold("Left")) { vec = Math::Vector3::Left; }
	if (InputManager::GetInstance().IsHold("Right")) { vec = Math::Vector3::Right; }

	if (vec.LengthSquared() > 0.0f)
	{
		owner.ChangeActionState(std::make_shared<ActionWalk>());
		return;
	}

	if (owner.m_isMasked)
	{
		owner.ChangeActionState(std::make_shared<ActionMasked>());
		return;
	}
}

void Mouse::ActionIdle::Exit(Mouse& owner)
{
}

//////////////////////////////////////////////////////////////////////////////
// 歩き状態
void Mouse::ActionWalk::Enter(Mouse& owner)
{
	if (!owner.m_isMasked)
	{
		owner.m_spAnimator->SetAnimation(owner.m_spOriginalModel->GetModelData()->GetAnimation("Walk"));
		owner.m_spAnimator->ResetAdvanceTime();
	}
}

void Mouse::ActionWalk::Update(Mouse& owner)
{
	auto time = ServiceLocator::Get<Time>();
	float speed = owner.m_speed * time->DeltaTime();

	Math::Vector3 move = Math::Vector3::Zero;
	if (InputManager::GetInstance().IsHold("Up"))
	{
		Math::Vector3 vec = Math::Vector3::Backward;
		move += vec;
	}
	if (InputManager::GetInstance().IsHold("Down"))
	{
		Math::Vector3 vec = Math::Vector3::Forward;
		move += vec;
	}
	if (InputManager::GetInstance().IsHold("Left"))
	{
		Math::Vector3 vec = Math::Vector3::Left;
		move += vec;
	}
	if (InputManager::GetInstance().IsHold("Right"))
	{
		Math::Vector3 vec = Math::Vector3::Right;
		move += vec;
	}

	if (owner.m_isMasked)
	{
		owner.ChangeActionState(std::make_shared<ActionMasked>());
		return;
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
	move *= owner.m_speed;
	move *= time->DeltaTime();

	owner.m_pos += move;

	owner.UpdateRotate(move);
}

void Mouse::ActionWalk::Exit(Mouse& owner)
{
}

void Mouse::ActionMasked::Enter(Mouse& owner)
{
	if (!EffekseerManager::GetInstance().IsPlaying("SmokeBomb.efk"))
	{
		owner.m_spEffect = EffekseerManager::GetInstance().Play("SmokeBomb.efk", owner.m_pos, false);
	}

	owner.m_spModel = owner.m_maskedModel;
}

void Mouse::ActionMasked::Update(Mouse& owner)
{
	owner.m_upCollider->SetEnable(Collider::Type::Sight, false);

	if (InputManager::GetInstance().IsPress("RClick"))
	{
		owner.m_isMasked = false;
		owner.m_spModel = owner.m_spOriginalModel;
	}

	if (!owner.m_isMasked)
	{
		owner.ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}
}

void Mouse::ActionMasked::Exit(Mouse& owner)
{
	owner.m_upCollider->SetEnable(Collider::Type::Sight, true);
}
