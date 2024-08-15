#include "Collider.h"

bool Collider::Intersects(const SphereInfo& targetShape, const Math::Matrix& ownerMat, std::list<Collider::CollisionResult>* pResults) const
{
	// �Փ˔��肪�����^�C�v�̏ꍇ�������^�[��
	if (targetShape.m_type & m_disableType) { return false; }

	bool isHit = false;

	for (auto& collisionShape : m_collisionShapes)
	{
		if (!(targetShape.m_type & collisionShape.second->GetType())) { continue; }
		
		Collider::CollisionResult tmpRes;
		Collider::CollisionResult* pTmpRes = pResults ? &tmpRes : nullptr;

		if (collisionShape.second->Intersects(targetShape.m_sphere, ownerMat, pTmpRes))
		{
			isHit = true;

			// �ڍׂȏՓˌ��ʂ��K�v�Ȃ��ꍇ��1�ł��ڐG���ĕԂ�
			if (!pResults) { break; }

			pResults->push_back(tmpRes);
		}
	}

	return isHit;
}

bool Collider::Intersects(const RayInfo& targetShape, const Math::Matrix& ownerMat, std::list<Collider::CollisionResult>* pResults) const
{
	// �Փ˔��肪�����^�C�v�̏ꍇ�������^�[��
	if (targetShape.m_type & m_disableType) { return false; }

	// ���C�̕����x�N�g�������݂��Ȃ��ꍇ����s�\�Ȃ̂ő������^�[��
	if (!targetShape.m_dir.LengthSquared())
	{
		assert(0 && "Collider::Intersects:���C�̕����x�N�g�������݂��܂���");
	
		return false;
	}

	bool isHit = false;

	for (auto& collisionShape : m_collisionShapes)
	{
		if (!(targetShape.m_type & collisionShape.second->GetType())) { continue; }
		
		Collider::CollisionResult tmpRes;
		Collider::CollisionResult* pTmpRes = pResults ? &tmpRes : nullptr;

		if (collisionShape.second->Intersects(targetShape, ownerMat, pTmpRes))
		{
			isHit = true;

			// �ڍׂȏՓˌ��ʂ��K�v�Ȃ��ꍇ��1�ł��ڐG���ĕԂ�
			if (!pResults) { break; }

			pResults->push_back(tmpRes);
		}
	}

	return isHit;
}

void Collider::SetEnable(std::string_view name, bool isEnable)
{
	auto targetCol = m_collisionShapes.find(name.data());
	if (targetCol != m_collisionShapes.end())
	{
		targetCol->second->SetEnable(isEnable);
	}
}

void Collider::SetEnable(int type, bool isEnable)
{
	if (isEnable)
	{
		m_disableType &= ~type;
	}
	else
	{
		m_disableType |= type;
	}
}

void Collider::SetEnableAll(bool isEnable)
{
	for (auto& col : m_collisionShapes)
	{
		col.second->SetEnable(isEnable);
	}
}

//=================================================================================================================================
// SphereCollision
// ���`�̌`��
//=================================================================================================================================

// ��vs���̏Փ˔���
bool SphereCollision::Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pRes)
{
	if (!m_isEnable) { return false; }

	DirectX::BoundingSphere shape;

	m_shape.Transform(shape, world);

	bool isHit = shape.Intersects(target);

	// �ڍׂ��K�v�Ȃ��ꍇ
	if (!pRes) { return isHit; }

	// �Փˎ��̂݌v�Z
	if (isHit)
	{
		// �݂��ɓ�����Ȃ��ŏ�����
		float needDistance = target.Radius + shape.Radius;
		
		// ���g���瑊��Ɍ����������x�N�g��
		pRes->m_hitDir = (Math::Vector3(target.Center) - Math::Vector3(shape.Center));
		float betweenDistance = pRes->m_hitDir.Length();

		// �d�Ȃ�� = �݂��ɓ�����Ȃ��ŏ����� - �݂��̎��ۋ���
		pRes->m_overlapDistance = needDistance - betweenDistance;

		pRes->m_hitDir.Normalize();

		// �݂��̋��̏Փ˂̒��S�_
		pRes->m_hitPos = shape.Center + pRes->m_hitDir * (shape.Radius + pRes->m_overlapDistance * 0.5f);
	}

	return isHit;
}

// ��vs���C�̏Փ˔���
bool SphereCollision::Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pRes)
{
	if (!m_isEnable) { return false; }

	DirectX::BoundingSphere shape;

	m_shape.Transform(shape, world);

	float hitDistance = 0.0f;

	bool isHit = shape.Intersects(target.m_pos,target.m_dir,hitDistance);

	// ������E����������
	isHit &= (target.m_range >= hitDistance);

	// �ڍׂ��K�v�Ȃ��ꍇ
	if (!pRes) { return isHit; }

	// �Փˎ��̂݌v�Z
	if (isHit)
	{
		// ���C���ˈʒu + ���C�̓��������ʒu�܂ł̃x�N�g��	
		pRes->m_hitPos = target.m_pos + target.m_dir * hitDistance;

		pRes->m_hitDir = target.m_dir * (-1);

		pRes->m_overlapDistance = target.m_range - hitDistance;
	}

	return isHit;
}
