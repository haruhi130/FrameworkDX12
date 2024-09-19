#include "Wolf.h"

#include "Application/Application.h"

void Wolf::Update()
{
	if (!m_spModel) { return; }

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

	m_spAnimator->ProgressTime(m_spModel->WorkNodes(),0.5f);
	m_spModel->CalcNodeMatrices();
}

void Wolf::Draw()
{
	if (!m_spModel) { return; }

	// モデル描画
	Application::GetInstance().GetShader().DrawModel(*m_spModel, m_mWorld);
}

void Wolf::OnHit()
{
	if (!m_isSight)
	{
		m_isSight = true;
	}
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
		m_spModel->SetModelData("Assets/Models/Wolf/Wolf.gltf");
	}

	// 初期計算
	Math::Matrix mScale = Math::Matrix::CreateScale(15.0f);
	Math::Matrix mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(0));
	m_mWorld = mScale * mRot;

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
		Collider::SphereInfo sphereInfo;
		sphereInfo.m_sphere.Center = GetPos() + Math::Vector3(0, 1.5f, 0);
		sphereInfo.m_sphere.Radius = 5.0f;
		sphereInfo.m_type = Collider::Type::Sight;

		for (std::weak_ptr<BaseObject> wpObj : m_wpHitObjList)
		{
			std::shared_ptr<BaseObject> spObj = wpObj.lock();
			if (spObj)
			{
				std::list<Collider::CollisionResult> retSightList;
				spObj->Intersects(sphereInfo, &retSightList);

				bool isHit = false;

				// 視界球の中
				for (auto& ret : retSightList)
				{
					// 前方向からsightAngle内にターゲットがいるか判定
					// 向いている方向
					Math::Vector3 nowDir = GetMatrix().Backward();
					nowDir.Normalize();
					float nowAng = DirectX::XMConvertToDegrees(atan2(nowDir.x, nowDir.z));

					// ターゲットの位置
					Math::Vector3 targetDir = ret.m_hitDir;
					targetDir.Normalize();
					float targetAng = DirectX::XMConvertToDegrees(atan2(targetDir.x, targetDir.z));

					// ターゲットの角度と向いている角度を計算
					float betweenAng = targetAng - nowAng;

					// sightAngle内か判定
					if (betweenAng < m_sightAngle && betweenAng > -m_sightAngle)
					{
						// ターゲットとの間に障害物がないか判定
						// 
						// Ray判定を作成する

						isHit = true;
					}
				}

				// 当たっている
				if (isHit)
				{
					OnHit();
				}
				else
				{
					m_isSight = false;
				}
			}
		}
	}

	// Sphere : Bump
	{
		Collider::SphereInfo sphereInfo;
		sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 1.0f, 0);
		sphereInfo.m_sphere.Radius = 0.8f;
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
					ret.m_hitDir.y = 0;
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
	float spd = 1.0f * time->DeltaTime();

	vec *= spd;

	owner.m_pos.z += vec.z;
}

void Wolf::ActionWalk::Exit(Wolf& owner)
{
}
