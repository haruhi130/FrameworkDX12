#include "Wolf.h"

#include "Application/Application.h"

void Wolf::Update()
{

	UpdateCollision();

	Math::Matrix mScale = Math::Matrix::CreateScale(10.0f);
	m_mWorld = mScale;
}

void Wolf::PostUpdate()
{
	auto time = ServiceLocator::Get<Time>();
	m_spAnimator->ProgressTime(m_spModel->WorkNodes(),1.5f);
	m_spModel->CalcNodeMatrices();
}

void Wolf::Draw()
{
	// ÉÇÉfÉãï`âÊ
	Application::GetInstance().GetShader().DrawModel(*m_spModel, m_mWorld);
}

void Wolf::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<ModelWork>();
		m_spModel->SetModelData("Assets/Models/Wolf/Wolf.gltf");
	}

	// èâä˙åvéZ
	Math::Matrix mScale = Math::Matrix::CreateScale(10.0f);
	m_mWorld = mScale;

	if (!m_spAnimator)
	{
		m_spAnimator = std::make_shared<Animator>();
		m_spAnimator->SetAnimation(m_spModel->GetAnimation("Idle"));
	}
}

void Wolf::UpdateCollision()
{
	{
		Collider::SphereInfo sphereInfo;
		sphereInfo.m_sphere.Center = GetPos() + Math::Vector3(0, 0.5f, 0);
		sphereInfo.m_sphere.Radius = 0.5f;
		sphereInfo.m_type = Collider::Type::Sight;

		for (std::weak_ptr<BaseObject> wpObj : m_wpHitObjList)
		{
			std::shared_ptr<BaseObject> spObj = wpObj.lock();
			if (spObj)
			{
				std::list<Collider::CollisionResult> retBumpList;
				spObj->Intersects(sphereInfo, &retBumpList);

				for (auto& ret : retBumpList)
				{
					m_spAnimator->SetAnimation(m_spModel->GetAnimation("Run"));
				}
			}
		}
	}
}
