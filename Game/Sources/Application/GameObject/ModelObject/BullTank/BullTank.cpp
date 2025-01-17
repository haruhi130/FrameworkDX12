#include "BullTank.h"
#include "../../../Scene/SceneManager.h"

void BullTank::Update()
{
	if (!m_spModel) { return; }

	// 重力処理
	m_pos.y -= m_gravity;
	m_gravity += 0.005f;

	if (m_sightTime <= 0)
	{
		m_sightTime = 0.0f;
	}

	if (m_sightTime >= 60.0f * 3)
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Failed);
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

	m_spAnimator->ProgressTime(m_spModel->WorkNodes());
	m_spModel->CalcNodeMatrices();
}

void BullTank::OnHit(bool isHit)
{
	m_isSight = isHit;
}

void BullTank::ImGuiUpdate()
{
	ImGui::Begin(u8"Wolf視界");
	ImGui::SetWindowSize(ImVec2(100, 100), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Checkbox(u8"有効", &m_isSight);
	ImGui::LabelText("sightTime", "ST : %f", m_sightTime);

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

	m_pos = { -5,0,13 };

	// 初期計算
	Math::Matrix mScale = Math::Matrix::CreateScale(200.0f);
	Math::Matrix mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(0));
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = mScale * mRot * mTrans;

	if (!m_spAnimator)
	{
		m_spAnimator = std::make_shared<Animator>();
	}

	// ステート設定「待機」
	ChangeActionState(std::make_shared<ActionWalk>());
}

void BullTank::UpdateMatrix()
{
	Math::Matrix mScale = Math::Matrix::CreateScale(200.0f);
	Math::Matrix mRot = Math::Matrix::Identity;
	mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(0));
	
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(m_pos);
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
		betweenAng -= 360;
	}
	else if (betweenAng < -180)
	{
		betweenAng += 360;
	}

	float rotAng = std::clamp(betweenAng, -36.0f, 36.0f);

	m_rot.y += rotAng;
}

void BullTank::UpdateCollision()
{
	// Sphere : Sight
	{
		// 視界範囲となる球判定を作成
		Collider::SphereInfo sphereInfo;
		sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 1.5f, 0);
		sphereInfo.m_sphere.Radius = 5.0f;
		sphereInfo.m_type = Collider::Type::Sight;

		for (std::weak_ptr<ModelObject> wpObj : m_wpObjList)
		{
			std::shared_ptr<ModelObject> spObj = wpObj.lock();
			if (spObj)
			{
				std::list<Collider::CollisionResult> retSightList;
				spObj->Intersects(sphereInfo, &retSightList);

				bool isHit = false;

				for (auto& ret : retSightList)
				{
					// 視界角度内に範囲を制限
					// 向いている方向
					Math::Vector3 nowDir = GetMatrix().Backward();
					nowDir.Normalize();
					float nowAng = DirectX::XMConvertToDegrees(atan2(nowDir.x, nowDir.z));

					// ターゲットの位置
					Math::Vector3 targetDir = ret.m_hitDir;
					targetDir.Normalize();
					float targetAng = DirectX::XMConvertToDegrees(atan2(targetDir.x, targetDir.z));

					// 角度計算
					float betweenAng = targetAng - nowAng;

					// sightAngle内か判定
					if (betweenAng <= m_sightAngle && betweenAng > -m_sightAngle)
					{
						// 間に障害物がないか判定
						Collider::RayInfo rayInfo;
						rayInfo.m_dir = targetDir - nowDir;
						rayInfo.m_dir.Normalize();
						rayInfo.m_pos = m_pos;
						rayInfo.m_range = sphereInfo.m_sphere.Radius;
						rayInfo.m_type = Collider::Type::Bump;

						if (spObj->Intersects(rayInfo, nullptr))
						{
							isHit = false;
						}
						else
						{
							isHit = true;
						}
					}
				}

				OnHit(isHit);
			}
		}
	}

	// Ray : Ground
	{
		Collider::RayInfo rayInfo;
		rayInfo.m_pos = m_pos;
		rayInfo.m_pos.y += 0.3f;

		rayInfo.m_dir = Math::Vector3::Down;
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
		sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 1.6f, 0);
		sphereInfo.m_sphere.Radius = 1.6f;
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
	++owner.m_sightTime;
	if (!owner.m_isSight)
	{
		owner.ChangeActionState(std::make_shared<ActionWalk>());
	}
}

void BullTank::ActionIdle::Exit(BullTank& owner)
{
}

void BullTank::ActionWalk::Enter(BullTank& owner)
{
	owner.m_spAnimator->SetAnimation(owner.m_spModel->GetModelData()->GetAnimation("Idle"));
}

void BullTank::ActionWalk::Update(BullTank& owner)
{
	if (owner.m_isSight)
	{
		owner.ChangeActionState(std::make_shared<ActionIdle>());
	}

	--owner.m_sightTime;

	Math::Vector3 vec = Math::Vector3::Zero;
	
	owner.UpdateRotate(vec);
}

void BullTank::ActionWalk::Exit(BullTank& owner)
{
}
