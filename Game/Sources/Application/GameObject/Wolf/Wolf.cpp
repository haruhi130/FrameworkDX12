#include "Wolf.h"

void Wolf::Update()
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

void Wolf::PostUpdate()
{
	if (!m_spModel) { return; }
	if (!m_spAnimator) { return; }

	m_spAnimator->ProgressTime(m_spModel->WorkNodes(),1.5f);
	m_spModel->CalcNodeMatrices();
}

void Wolf::Draw()
{
	if (!m_spModel) { return; }

	// モデル描画
	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void Wolf::OnHit(bool isHit)
{
	m_isSight = isHit;
}

void Wolf::ImGuiUpdate()
{
	ImGui::Begin(u8"Wolf視界");
	ImGui::SetWindowSize(ImVec2(100, 100), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Checkbox(u8"有効", &m_isSight);

	ImGui::End();
}

void Wolf::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData(Assets::GetInstance().m_modelDatas.
			GetData("Assets/Models/Wolf/Wolf.gltf"));
	}

	m_pos = { 0,0,20 };

	// 初期計算
	Math::Matrix mScale = Math::Matrix::CreateScale(15.0f);
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

void Wolf::UpdateMatrix()
{
	Math::Matrix mScale = Math::Matrix::CreateScale(15.0f);
	Math::Matrix mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(0));
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = mScale * mRot * mTrans;
}

void Wolf::UpdateCollision()
{
	// Sphere : Sight
	{
		// 視界範囲となる球判定を作成
		Collider::SphereInfo sphereInfo;
		sphereInfo.m_sphere.Center = GetPos() + Math::Vector3(0, 1.5f, 0);
		sphereInfo.m_sphere.Radius = 7.0f;
		sphereInfo.m_type = Collider::Type::Sight;

		for (std::weak_ptr<BaseObject> wpObj : m_wpHitObjList)
		{
			std::shared_ptr<BaseObject> spObj = wpObj.lock();
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
						isHit = true;
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
		sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 1.5f, 0);
		sphereInfo.m_sphere.Radius = 1.6f;
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
}

void Wolf::ChangeActionState(std::shared_ptr<ActionStateBase> nextState)
{
	if (m_currentAction)m_currentAction->Exit(*this);
	m_currentAction = nextState;
	m_currentAction->Enter(*this);
}

void Wolf::ActionIdle::Enter(Wolf& owner)
{
	owner.m_spAnimator->SetAnimation(owner.m_spModel->GetModelData()->GetAnimation("Idle"));
}

void Wolf::ActionIdle::Update(Wolf& owner)
{
	if (!owner.m_isSight)
	{
		owner.ChangeActionState(std::make_shared<ActionWalk>());
	}
}

void Wolf::ActionIdle::Exit(Wolf& owner)
{
}

void Wolf::ActionWalk::Enter(Wolf& owner)
{
	owner.m_spAnimator->SetAnimation(owner.m_spModel->GetModelData()->GetAnimation("Run"));
}

void Wolf::ActionWalk::Update(Wolf& owner)
{
	if (owner.m_isSight)
	{
		owner.ChangeActionState(std::make_shared<ActionIdle>());
	}

	Math::Vector3 vec = Math::Vector3::Zero;
	vec.z = -1.0f;
	vec.Normalize();

	auto time = ServiceLocator::Get<Time>();
	float spd = 4.0f * time->DeltaTime();

	vec *= spd;

	owner.m_pos.z += vec.z;
}

void Wolf::ActionWalk::Exit(Wolf& owner)
{
}
