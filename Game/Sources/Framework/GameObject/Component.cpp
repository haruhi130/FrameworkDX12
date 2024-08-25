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
}

void ModelComponent::SetModel(std::shared_ptr<ModelData> model)
{
}
