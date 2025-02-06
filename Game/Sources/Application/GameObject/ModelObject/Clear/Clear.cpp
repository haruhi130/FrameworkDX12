#include "Clear.h"
#include "../../../Scene/SceneManager.h"

void Clear::Update()
{
	if (SceneManager::GetInstance().GetGoalFlg())
	{
		if (!EffekseerManager::GetInstance().IsPlaying("Heal.efk"))
		{
			m_spEffect = EffekseerManager::GetInstance().Play("Heal.efk", m_pos);
			m_spEffect->SetScale(0.5f);
		}
	}
}

void Clear::Init()
{
	m_upCollider = std::make_unique<Collider>();
	m_upCollider->RegisterCollisionShape("Clear", m_pos, 1.0f, Collider::Type::Clear);
}
