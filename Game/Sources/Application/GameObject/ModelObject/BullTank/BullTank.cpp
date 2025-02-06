#include "BullTank.h"
#include "../../../Scene/SceneManager.h"

void BullTank::Update()
{
	if (!m_spModel) { return; }

	// 重力処理
	m_mWorld._42 -= m_gravity;
	m_gravity += 0.005f;

	m_sightTime = std::clamp(m_sightTime, 0.0f, 60.0f * 3);

	if (m_sightTime >= 60.0f * 3)
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Failed);
	}

	m_sightAngle = std::clamp(m_sightAngle, 0.1f, 90.0f);

	if (SceneManager::GetInstance().GetGoalFlg())
	{
		m_sightRange = 8.0f;
		m_sightAngle = 60.0f;
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

void BullTank::PostUpdate()
{
	if (!m_spModel) { return; }
	if (!m_spAnimator) { return; }

	auto vec = GetMatrix().Forward();
	vec.Normalize();
	if (m_isSight)
	{
	ShaderManager::GetInstance().AddSpotLight({ 1,0,0 },m_sightRange, GetPos() + Math::Vector3(0, 1.5, 0), DirectX::XMConvertToRadians(m_sightAngle * 2), vec);
	}
	else
	{
		ShaderManager::GetInstance().AddSpotLight({ 1,1,1 }, m_sightRange, GetPos() + Math::Vector3(0, 1.5, 0), DirectX::XMConvertToRadians(m_sightAngle * 2), vec);
	}

	m_spAnimator->ProgressTime(m_spModel->WorkNodes());
	m_spModel->CalcNodeMatrices();
}

void BullTank::OnHit(bool isHit)
{
	m_isSight = isHit;
}

void BullTank::ImGuiUpdate()
{
	ImGui::Begin(u8"BullTank");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(200, 300));
	ImGui::Checkbox(u8"有効", &m_isSight);
	ImGui::LabelText("sightTime", "ST : %.2f", m_sightTime);
	ImGui::LabelText("pos", "%.2f,%.2f,%.2f", GetPos().x, GetPos().y, GetPos().z);
	ImGui::LabelText("scale", "%.2f", m_scale);

	ImGui::End();
}

void BullTank::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.
			GetData("Assets/Models/BullTank/BullTank.gltf"));
	}

	if (!m_spAnimator)
	{
		m_spAnimator = std::make_shared<Animator>();
	}

	// ステート設定「待機」
	ChangeActionState(std::make_shared<ActionIdle>());

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("BullTank", m_spModel, Collider::Type::Bump);
}

void BullTank::UpdateMatrix()
{
	Math::Matrix mScale = Math::Matrix::CreateScale(m_scale);
	Math::Matrix mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rot.y));

	Math::Matrix mTrans = Math::Matrix::CreateTranslation(GetPos());
	m_mWorld = mScale * mRot * mTrans;
}

void BullTank::UpdateRotate(Math::Vector3& moveVec)
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
		betweenAng -= 90;
	}
	else if (betweenAng < -180)
	{
		betweenAng += 90;
	}

	float rotAng = std::clamp(betweenAng, -180.0f, 180.0f);

	m_rot.y += rotAng;
}

void BullTank::UpdateCollision()
{
	Math::Vector3 pos = GetPos();

	// Ray : Ground
	{
		Collider::RayInfo rayInfo;
		rayInfo.m_pos = pos;
		rayInfo.m_pos.y += 1.0f;

		rayInfo.m_dir = Math::Vector3::Down;
		rayInfo.m_dir.Normalize();
		rayInfo.m_range = m_gravity + 1.0f;

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
					SetPos(hitPos);
					m_gravity = 0.0f;
					m_isGround = true;
				}
			}
		}
	}

	Math::Vector3 hitPos = Math::Vector3::Zero;
	Math::Vector3 hitDir = Math::Vector3::Zero;
	bool isHit = false;

	// Sphere : Sight
	{
		Collider::SphereInfo sphereSight;
		sphereSight.m_sphere.Center = pos + Math::Vector3(0, 1.5f, 0);
		sphereSight.m_sphere.Radius = m_sightRange;
		sphereSight.m_type = Collider::Type::Sight;

		std::list<Collider::CollisionResult> retSphereSightList;

		for (std::weak_ptr<ModelObject> wpObj : m_wpObjList)
		{
			std::shared_ptr<ModelObject> spObj = wpObj.lock();
			if (spObj)
			{
				if (spObj->Intersects(sphereSight, &retSphereSightList))
				{
					for (auto&& retSphereSight : retSphereSightList)
					{
						hitPos = retSphereSight.m_hitPos;
						hitDir = retSphereSight.m_hitDir;
					}

					// 視界角度内に範囲を制限
					// 向いている方向
					Math::Vector3 nowDir = GetMatrix().Backward();
					nowDir.Normalize();
					float nowAng = DirectX::XMConvertToDegrees(atan2(nowDir.x, nowDir.z));

					// ターゲットの位置
					hitDir.Normalize();
					float targetAng = DirectX::XMConvertToDegrees(atan2(hitDir.x, hitDir.z));

					// 角度計算
					float betweenAng = targetAng - nowAng;

					if (-m_sightAngle <= betweenAng && betweenAng <= m_sightAngle)
					{
						isHit = true;
						continue;
					}
					else
					{
						isHit = false;
					}
				}
			}
		}
	}

	if (isHit)
	{
		Collider::RayInfo raySight;
		raySight.m_pos = pos + Math::Vector3(0, 1.5f, 0);
		raySight.m_dir = hitPos - raySight.m_pos;
		raySight.m_range = raySight.m_dir.Length();
		raySight.m_dir.Normalize();

		raySight.m_type = Collider::Type::Ground;

		for (std::weak_ptr<ModelObject> wpObj : m_wpObjList)
		{
			std::shared_ptr<ModelObject> spObj = wpObj.lock();
			if (spObj)
			{
				std::list<Collider::CollisionResult> retRaySightList;
				if (spObj->Intersects(raySight, &retRaySightList))
				{
					isHit = false;
					break;
				}
				else
				{
					isHit = true;
				}
			}
		}
	}

	OnHit(isHit);
}

void BullTank::ChangeActionState(std::shared_ptr<ActionStateBase> nextState)
{
	if (m_currentAction)m_currentAction->Exit(*this);
	m_currentAction = nextState;
	m_currentAction->Enter(*this);
}

void BullTank::ActionIdle::Enter(BullTank& owner)
{
	owner.m_spAnimator->SetAnimation(owner.m_spModel->GetModelData()->GetAnimation("Idle"));
}

void BullTank::ActionIdle::Update(BullTank& owner)
{
	--owner.m_sightTime;

	if (owner.m_sightTime <= 0.0f && owner.m_isMove)
	{
		owner.ChangeActionState(std::make_shared<ActionWalk>());
	}

	if (owner.m_isSight)
	{
		owner.ChangeActionState(std::make_shared<ActionSight>());
		return;
	}
}

void BullTank::ActionIdle::Exit(BullTank& owner)
{
}

void BullTank::ActionWalk::Enter(BullTank& owner)
{
	owner.m_spAnimator->ResetAdvanceTime();
	owner.m_spAnimator->SetAnimation(owner.m_spModel->GetModelData()->GetAnimation("Walk"));
}

void BullTank::ActionWalk::Update(BullTank& owner)
{
	--owner.m_sightTime;

	if (owner.m_isSight)
	{
		owner.ChangeActionState(std::make_shared<ActionSight>());
		return;
	}

	auto time = ServiceLocator::Get<Time>();
	float speed = (1.0f / (60.0f * owner.m_speed));
	speed *= time->DeltaTime();
	Math::Vector3 rot = Math::Vector3::Zero;
	owner.m_progress += speed * (owner.m_isRevers * -2 + 1);
	if (owner.m_progress > 1.1f)
	{
		rot = owner.m_endPos - owner.m_startPos;
		owner.m_isRevers = true;
	}
	if (owner.m_progress <= -0.1f)
	{
		rot = owner.m_startPos - owner.m_endPos;
		owner.m_isRevers = false;
	}
	float progress = std::clamp(owner.m_progress, 0.0f, 1.0f);
	Math::Vector3 towardEndVec = owner.m_endPos - owner.m_startPos;

	Math::Vector3 vec = owner.m_startPos + towardEndVec * EaseInOutSine(progress);

	owner.SetPos(vec);

	rot.Normalize();
	owner.UpdateRotate(rot);
}

void BullTank::ActionWalk::Exit(BullTank& owner)
{
}

void BullTank::ActionSight::Enter(BullTank& owner)
{
	if (owner.m_sightTime <= 0)
	{
		if (!owner.m_spSound)
		{
			owner.m_spSound = AudioManager::GetInstance().Play("Assets/Sounds/busted.wav");
		}
		if (!owner.m_spSound->IsPlaying())
		{
			owner.m_spSound->Play();
		}

		owner.m_spSound->SetVolume(SceneManager::GetInstance().GetSEVolume());
	}
	owner.m_spAnimator->SetAnimation(owner.m_spModel->GetModelData()->GetAnimation("Idle"));
}

void BullTank::ActionSight::Update(BullTank& owner)
{
	++owner.m_sightTime;

	if (!owner.m_isSight)
	{
		owner.ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}
}

void BullTank::ActionSight::Exit(BullTank& owner)
{
}
