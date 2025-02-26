#include "Mouse.h"
#include "../../Camera/GameCamera.h"
#include "../../../Scene/SceneManager.h"

void Mouse::Draw()
{
	if (!m_spModel) { return; }

	ShaderManager::GetInstance().m_modelShader.SetDitherEnable();
	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void Mouse::DrawShadow()
{
	if (!m_spModel) { return; }
	ShaderManager::GetInstance().m_modelShader.SetDitherEnable();
	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld, false);
}

void Mouse::Update()
{
	if (!m_spModel) { return; }

	// �d�͏���
	m_mWorld._42 -= m_gravity;
	m_gravity += 0.005f;

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

void Mouse::PostUpdate()
{
	if (!m_spModel) { return; }
	if (!m_spAnimator) { return; }

	if (!m_isMasked)
	{
		// �A�j���[�V��������
		m_spAnimator->ProgressTime(m_spOriginalModel->WorkNodes());
		m_spOriginalModel->CalcNodeMatrices();
	}
}

void Mouse::Init()
{
	// ���f���ǂݍ���
	if (!m_spOriginalModel)
	{
		m_spOriginalModel = std::make_shared<ModelWork>();
		m_spOriginalModel->SetModelData(Assets::GetInstance().m_modelDatas.
			GetData("Assets/Models/Mouse/Mouse.gltf"));
		m_spModel = m_spOriginalModel;
	}

	m_speed = 3.0f;

	// �A�j���[�V�����ݒ�
	if (!m_spAnimator)
	{
		m_spAnimator = std::make_shared<Animator>();
	}

	// �X�e�[�g�ݒ�u�ҋ@�v
	ChangeActionState(std::make_shared<ActionIdle>());

	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Mouse", m_spOriginalModel, Collider::Type::Bump | Collider::Type::Sight);
}

void Mouse::UpdateMatrix()
{
	Math::Matrix mScale = Math::Matrix::CreateScale(m_scale);
	Math::Matrix mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_rotateVec.y));
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(GetPos());
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
	if (betweenAng > 180.0f)
	{
		betweenAng -= 360.0f;
	}
	else if (betweenAng < -180.0f)
	{
		betweenAng += 360.0f;
	}

	float rotAng = std::clamp(betweenAng, -36.0f, 36.0f);

	m_rotateVec.y += rotAng;
}

void Mouse::UpdateCollision()
{
	Math::Vector3 pos = GetPos();

	// Ray : Ground
	{
		Collider::RayInfo rayInfo;
		rayInfo.m_pos = pos;
		rayInfo.m_pos.y += 0.5f;

		rayInfo.m_dir = Math::Vector3::Down;
		rayInfo.m_dir.Normalize();
		rayInfo.m_range = m_gravity + 0.5f;

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

	// Sphere : Bump
	{
		Collider::SphereInfo sphereInfo;
		sphereInfo.m_sphere.Center = pos + Math::Vector3(0, 0.7f, 0);
		sphereInfo.m_sphere.Radius = 0.7f;
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
					Math::Vector3 newPos = Math::Vector3::Zero;
					newPos = (ret.m_hitDir * ret.m_overlapDistance);
					pos += newPos;
					SetPos(pos);
				}
			}
		}
	}

	// Ray : Event
	if (InputManager::GetInstance().IsPress("LClick") && m_coolTime <= 0)
	{
		// �}�E�X���W�擾
		POINT mousePos = {};
		GetCursorPos(&mousePos);

		// �J�������
		const std::shared_ptr<GameCamera> spCamera = m_wpCamera.lock();
		if (spCamera)
		{
			//--------------------------------------------------------------
			// 2D���W����3D���W�ɕϊ�
			// ���C���ɕK�v�ȕϐ��̏�����
			Math::Vector3 rayDir = Math::Vector3::Zero;
			float range = 0.0f;

			// �J�������W�擾
			Math::Vector3 cameraPos = spCamera->GetPos();

			// �}�E�X���W(2D)����J����(3D)�ւ̃��C�����쐬
			spCamera->WorkCamera()->GenerateRayInfoFromClient(
				mousePos, cameraPos, rayDir, range);

			//--------------------------------------------------------------
			// ���C���𓖂��蔻��ɐݒ�
			Collider::RayInfo rayInfo;
			rayInfo.m_type = Collider::Type::Event;
			rayInfo.m_pos = cameraPos;
			rayInfo.m_dir = rayDir;
			rayInfo.m_dir.Normalize();
			rayInfo.m_range = 10.0f;

			for (std::weak_ptr<ModelObject> wpObj : m_wpObjList)
			{
				std::shared_ptr<ModelObject> spObj = wpObj.lock();
				if (spObj)
				{
					std::list<Collider::CollisionResult> retRayList;
					spObj->Intersects(rayInfo, &retRayList);

					for (auto& ret : retRayList)
					{
						m_maskedModel = spObj->GetModel();
						m_isMasked = true;
						m_coolTime = 60.0f * 3;
					}
				}
			}
		}
	}

	// Sphere : Goal
	{
		Collider::SphereInfo sphereInfo;
		sphereInfo.m_sphere.Center = pos + Math::Vector3(0, 0.5f, 0);
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
						SceneManager::GetInstance().SetGoalFlg(true);
						m_spSound = AudioManager::GetInstance().Play("Assets/Sounds/Get.wav");
						m_spSound->SetVolume(SceneManager::GetInstance().GetSEVolume());
					}
				}
			}
		}
	}

	// Sphere : Clear
	{
		Collider::SphereInfo sphereInfo;
		sphereInfo.m_sphere.Center = pos + Math::Vector3(0, 0.5f, 0);
		sphereInfo.m_sphere.Radius = 0.5f;
		sphereInfo.m_type = Collider::Type::Clear;

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
						if (SceneManager::GetInstance().GetGoalFlg())
						{
							SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Clear);
						}
					}
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
	if (!owner.m_isMasked)
	{
		owner.m_spAnimator->SetAnimation(owner.m_spOriginalModel->GetModelData()->GetAnimation("Idle"));
		owner.m_spAnimator->ResetAdvanceTime();
	}
}

void Mouse::ActionIdle::Update(Mouse& owner)
{
	--owner.m_coolTime;
	if (owner.m_coolTime <= 0)
	{
		owner.m_coolTime = 0.0f;
	}

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
// �������
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
	--owner.m_coolTime;
	if (owner.m_coolTime <= 0)
	{
		owner.m_coolTime = 0.0f;
	}

	auto time = ServiceLocator::Get<Time>();

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
	owner.UpdateRotate(move);
	
	Math::Vector3 pos = owner.GetPos();
	pos += (move * owner.m_speed) * time->DeltaTime();

	owner.SetPos(pos);
}

void Mouse::ActionWalk::Exit(Mouse& owner)
{
}

void Mouse::ActionMasked::Enter(Mouse& owner)
{
	if (!EffekseerManager::GetInstance().IsPlaying("SmokeBomb.efk"))
	{
		auto smokeSound = AudioManager::GetInstance().Play("Assets/Sounds/smoke.wav");
		smokeSound->SetVolume(SceneManager::GetInstance().GetSEVolume());
		owner.m_spEffect = EffekseerManager::GetInstance().Play("SmokeBomb.efk", owner.GetPos(), false);
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
