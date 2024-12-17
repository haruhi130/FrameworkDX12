#include "Wolf.h"
#include "../../../Scene/SceneManager.h"

void Wolf::Update()
{
	if (!m_spModel) { return; }

	// �d�͏���
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

	// �X�e�[�g�X�V
	if (m_currentAction)
	{
		m_currentAction->Update(*this);
	}

	// �s��X�V
	UpdateMatrix();
	// �����蔻��X�V
	UpdateCollision();
}

void Wolf::PostUpdate()
{
	if (!m_spModel) { return; }
	if (!m_spAnimator) { return; }

	m_spAnimator->ProgressTime(m_spModel->WorkNodes(), 1.5f);
	m_spModel->CalcNodeMatrices();
}

void Wolf::Draw()
{
	if (!m_spModel) { return; }

	// ���f���`��
	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void Wolf::DrawShadow()
{
	if (!m_spModel) { return; }

	// ���f���`��
	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void Wolf::OnHit(bool isHit)
{
	m_isSight = isHit;
}

void Wolf::ImGuiUpdate()
{
	ImGui::Begin(u8"Wolf���E");
	ImGui::SetWindowSize(ImVec2(100, 100), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Checkbox(u8"�L��", &m_isSight);
	ImGui::LabelText("sightTime", "ST : %f", m_sightTime);

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

	m_pos = { -5,0,20 };

	// �����v�Z
	Math::Matrix mScale = Math::Matrix::CreateScale(15.0f);
	Math::Matrix mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(0));
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = mScale * mRot * mTrans;

	if (!m_spAnimator)
	{
		m_spAnimator = std::make_shared<Animator>();
	}

	// �X�e�[�g�ݒ�u�ҋ@�v
	ChangeActionState(std::make_shared<ActionWalk>());
}

void Wolf::UpdateMatrix()
{
	Math::Matrix mScale = Math::Matrix::CreateScale(15.0f);
	Math::Matrix mRot = Math::Matrix::Identity;
	if (m_r >= 0)
	{
		mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(0));
	}
	else
	{
		mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180));
	}
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = mScale * mRot * mTrans;
}

void Wolf::UpdateRotate(Math::Vector3& moveVec)
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

void Wolf::UpdateCollision()
{
	// Sphere : Sight
	{
		// ���E�͈͂ƂȂ鋅������쐬
		Collider::SphereInfo sphereInfo;
		sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 1.5f, 0);
		sphereInfo.m_sphere.Radius = 5.0f;
		sphereInfo.m_type = Collider::Type::Sight;

		for (std::weak_ptr<BaseObject> wpObj : m_wpObjList)
		{
			std::shared_ptr<BaseObject> spObj = wpObj.lock();
			if (spObj)
			{
				std::list<Collider::CollisionResult> retSightList;
				spObj->Intersects(sphereInfo, &retSightList);

				bool isHit = false;

				for (auto& ret : retSightList)
				{
					// ���E�p�x���ɔ͈͂𐧌�
					// �����Ă������
					Math::Vector3 nowDir = GetMatrix().Backward();
					nowDir.Normalize();
					float nowAng = DirectX::XMConvertToDegrees(atan2(nowDir.x, nowDir.z));

					// �^�[�Q�b�g�̈ʒu
					Math::Vector3 targetDir = ret.m_hitDir;
					targetDir.Normalize();
					float targetAng = DirectX::XMConvertToDegrees(atan2(targetDir.x, targetDir.z));

					// �p�x�v�Z
					float betweenAng = targetAng - nowAng;

					// sightAngle��������
					if (betweenAng <= m_sightAngle && betweenAng > -m_sightAngle)
					{
						// �Ԃɏ�Q�����Ȃ�������
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

		for (std::weak_ptr<BaseObject> wpObj : m_wpObjList)
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
		sphereInfo.m_sphere.Center = m_pos + Math::Vector3(0, 1.6f, 0);
		sphereInfo.m_sphere.Radius = 1.6f;
		sphereInfo.m_type = Collider::Type::Bump;

		for (std::weak_ptr<BaseObject> wpObj : m_wpObjList)
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
	++owner.m_sightTime;
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

	--owner.m_sightTime;
	++owner.m_rotTime;

	if (owner.m_rotTime > 60.0f * 5)
	{
		owner.m_r *= -1;
		owner.m_rotTime = 0;
	}

	Math::Vector3 vec = Math::Vector3::Zero;
	vec.z = -0.5f * owner.m_r;
	vec.Normalize();

	auto time = ServiceLocator::Get<Time>();
	float spd = 4.0f * time->DeltaTime();

	vec *= spd;

	owner.m_pos.z += vec.z;

	owner.UpdateRotate(vec);
}

void Wolf::ActionWalk::Exit(Wolf& owner)
{
}
