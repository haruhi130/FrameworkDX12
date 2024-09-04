#include "Component.h"

void ModelComponent::Update()
{
	Component::Update();

	auto owner = m_wpOwner.lock();
	if (!owner) { return; }

	if (!m_spModelWork) { return; }
}

void ModelComponent::Draw()
{
	Shader shader;
	shader.DrawModel(*m_spModelWork);
}

void ModelComponent::SetModel(std::shared_ptr<ModelData> model)
{
	m_spModelWork = std::make_shared<ModelWork>();
	if (m_spModelWork)
	{
		m_spModelWork->SetModelData(model);
	}
}
