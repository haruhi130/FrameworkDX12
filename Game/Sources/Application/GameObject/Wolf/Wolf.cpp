#include "Wolf.h"

#include "Application/Application.h"

void Wolf::Update()
{
	// ステート更新
	if (m_currentAction)
	{
		m_currentAction->Update(*this);
	}

	Math::Matrix mScale = Math::Matrix::CreateScale(20.0f);
	m_mWorld = mScale;

	UpdateCollision();
}

void Wolf::PostUpdate()
{
	m_spAnimator->ProgressTime(m_spModel->WorkNodes(),1.5f);
	m_spModel->CalcNodeMatrices();
}

void Wolf::Draw()
{
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
	Math::Matrix mScale = Math::Matrix::CreateScale(20.0f);
	m_mWorld = mScale;

	if (!m_spAnimator)
	{
		m_spAnimator = std::make_shared<Animator>();
	}

	// ステート設定「待機」
	ChangeActionState(std::make_shared<ActionWalk>());
}

void Wolf::UpdateCollision()
{
	Collider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = GetPos() + Math::Vector3(0,1.5f,0);
	sphereInfo.m_sphere.Radius = 5.0f;
	sphereInfo.m_type = Collider::Type::Sight;

	for (std::weak_ptr<BaseObject> wpObj : m_wpHitObjList)
	{
		std::shared_ptr<BaseObject> spObj = wpObj.lock();
		if (spObj)
		{
			std::list<Collider::CollisionResult> retSightList;
			spObj->Intersects(sphereInfo, &retSightList);

			// 視界球の中
			for (auto& ret : retSightList)
			{
				OnHit();
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
}

void Wolf::ActionWalk::Exit(Wolf& owner)
{
}
