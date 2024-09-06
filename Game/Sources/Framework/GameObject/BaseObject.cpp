#include "BaseObject.h"

bool BaseObject::Intersects(const Collider::SphereInfo& target, std::list<Collider::CollisionResult>* pResults)
{
	if (!m_upCollider) { return false; }

	return m_upCollider->Intersects(target, m_mWorld, pResults);
}

bool BaseObject::Intersects(const Collider::RayInfo& target, std::list<Collider::CollisionResult>* pResults)
{
	if (!m_upCollider) { return false; }

	return m_upCollider->Intersects(target, m_mWorld, pResults);
}