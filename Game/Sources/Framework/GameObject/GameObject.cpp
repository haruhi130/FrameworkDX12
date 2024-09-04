#include "GameObject.h"
#include "Component.h"

void GameObject::Init()
{
	AddComponent<TransformComponent>();
}

void GameObject::Update()
{
	for (auto&& comp : m_spComponents)
	{
		comp->Update();
	}
	for (auto&& child : GetChildren())
	{
		child->Update();
	}
}

void GameObject::Draw()
{
	for (auto&& comp : m_spComponents)
	{
		comp->Draw();
	}
	for (auto&& child : GetChildren())
	{
		child->Draw();
	}
}

Math::Matrix& GameObject::Transform()
{
	auto& trans = *(m_spComponents.begin());
	return std::static_pointer_cast<TransformComponent>(trans)->Transform();
}

std::shared_ptr<GameObject> GameObject::GetParent()
{
	auto& trans = *(m_spComponents.begin());
	return std::static_pointer_cast<TransformComponent>(trans)->Parent();
}

std::list<std::shared_ptr<GameObject>> GameObject::GetChildren()
{
	auto& trans = *(m_spComponents.begin());
	return std::static_pointer_cast<TransformComponent>(trans)->Children();
}
