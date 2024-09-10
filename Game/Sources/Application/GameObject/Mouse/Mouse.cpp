#include "Mouse.h"
#include "Application/Application.h"

void Mouse::Update()
{
	if (m_currentAction)
	{
		m_currentAction->Update(*this);
	}
}

void Mouse::PostUpdate()
{
	m_spAnimator->ProgressTime(m_spModel->WorkNodes(),animeTime);
	m_spModel->CalcNodeMatrices();
}

void Mouse::Draw()
{
	Application::GetInstance().GetShader().DrawModel(*m_spModel, m_mWorld);
}

void Mouse::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData("Assets/Models/Mouse/Mouse.gltf");
	}

	Math::Matrix mRot = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90));
	Math::Matrix mScale = Math::Matrix::CreateScale(10000.0f);
	Math::Matrix mTrans = Math::Matrix::CreateTranslation(0, 0, 0);
	m_mWorld = mRot * mScale * mTrans;

	if (!m_spAnimator)
	{
		m_spAnimator = std::make_shared<Animator>();
		m_spAnimator->SetAnimation(m_spModel->GetAnimation("Idle"));
	}

	ChangeActionState(std::make_shared<ActionIdle>());
}

void Mouse::ChangeActionState(std::shared_ptr<ActionStateBase> nextState)
{
	if (m_currentAction)m_currentAction->Exit(*this);
	m_currentAction = nextState;
	m_currentAction->Enter(*this);
}

void Mouse::ActionIdle::Enter(Mouse& owner)
{
	owner.m_spAnimator->SetAnimation(owner.m_spModel->GetModelData()->GetAnimation("Idle"));
}

void Mouse::ActionIdle::Update(Mouse& owner)
{
	Math::Vector3 vec = Math::Vector3::Zero;
	if (GetAsyncKeyState('W')) { vec.z = 1.0f; }
	if (GetAsyncKeyState('S')) { vec.z = -1.0f; }
	if (GetAsyncKeyState('A')) { vec.x = -1.0f; }
	if (GetAsyncKeyState('D')) { vec.x = 1.0f; }

	if (vec.LengthSquared() > 0)
	{
		owner.ChangeActionState(std::make_shared<ActionWalk>());
	}
}

void Mouse::ActionIdle::Exit(Mouse& owner)
{
}

void Mouse::ActionWalk::Enter(Mouse& owner)
{
	owner.m_spAnimator->SetAnimation(owner.m_spModel->GetModelData()->GetAnimation("Walk"));
}

void Mouse::ActionWalk::Update(Mouse& owner)
{
	Math::Vector3 vec = Math::Vector3::Zero;
	if (GetAsyncKeyState('W')) { vec.z = 1.0f; }
	if (GetAsyncKeyState('S')) { vec.z = -1.0f; }
	if (GetAsyncKeyState('A')) { vec.x = -1.0f; }
	if (GetAsyncKeyState('D')) { vec.x = 1.0f; }

	if (vec.LengthSquared() == 0)
	{
		owner.ChangeActionState(std::make_shared<ActionIdle>());
	}
}

void Mouse::ActionWalk::Exit(Mouse& owner)
{
}
