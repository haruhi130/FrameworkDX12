#include "Wolf.h"

#include "Application/Application.h"

void Wolf::Update()
{
	if (!m_spModel) { return; }

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

	m_spAnimator->ProgressTime(m_spModel->WorkNodes(),0.5f);
	m_spModel->CalcNodeMatrices();
}

void Wolf::Draw()
{
	if (!m_spModel) { return; }

	// ���f���`��
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
	ImGui::Begin(u8"Wolf���E");
	ImGui::SetWindowSize(ImVec2(100, 100), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Checkbox(u8"�L��", &m_isSight);
	
	ImGui::End();
}

void Wolf::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData("Assets/Models/Wolf/Wolf.gltf");
	}

	// �����v�Z
	Math::Matrix mScale = Math::Matrix::CreateScale(15.0f);
	Math::Matrix mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(0));
	m_mWorld = mScale * mRot;

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

				// ���E���̒�
				for (auto& ret : retSightList)
				{
					// �O��������sightAngle���Ƀ^�[�Q�b�g�����邩����
					// �����Ă������
					Math::Vector3 nowDir = GetMatrix().Backward();
					nowDir.Normalize();
					float nowAng = DirectX::XMConvertToDegrees(atan2(nowDir.x, nowDir.z));

					// �^�[�Q�b�g�̈ʒu
					Math::Vector3 targetDir = ret.m_hitDir;
					targetDir.Normalize();
					float targetAng = DirectX::XMConvertToDegrees(atan2(targetDir.x, targetDir.z));

					// �^�[�Q�b�g�̊p�x�ƌ����Ă���p�x���v�Z
					float betweenAng = targetAng - nowAng;

					// sightAngle��������
					if (betweenAng < m_sightAngle && betweenAng > -m_sightAngle)
					{
						// �^�[�Q�b�g�Ƃ̊Ԃɏ�Q�����Ȃ�������
						// 
						// Ray������쐬����

						isHit = true;
					}
				}

				// �������Ă���
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
