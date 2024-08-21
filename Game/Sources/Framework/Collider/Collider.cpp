#include "Collider.h"

void Collider::RegisterCollisionShape(std::string_view name, std::unique_ptr<CollisionShape> spShape)
{
	if (!spShape) { return; }

	m_collisionShapes.emplace(name.data(), std::move(spShape));
}

void Collider::RegisterCollisionShape(std::string_view name, const DirectX::BoundingSphere& sphere, UINT type)
{
	RegisterCollisionShape(name, std::make_unique<SphereCollision>(sphere, type));
}

void Collider::RegisterCollisionShape(std::string_view name, const Math::Vector3& localPos, float radius, UINT type)
{
	RegisterCollisionShape(name, std::make_unique<SphereCollision>(localPos, radius, type));
}

void Collider::RegisterCollisionShape(std::string_view name, const std::shared_ptr<ModelData>& model, UINT type)
{
	RegisterCollisionShape(name, std::make_unique<ModelCollision>(model, type));
}

void Collider::RegisterCollisionShape(std::string_view name, ModelData* model, UINT type)
{
	RegisterCollisionShape(name, std::make_unique<ModelCollision>(std::shared_ptr<ModelData>(model), type));
}

void Collider::RegisterCollisionShape(std::string_view name, const std::shared_ptr<ModelWork>& model, UINT type)
{
	RegisterCollisionShape(name, std::make_unique<ModelCollision>(model, type));
}

void Collider::RegisterCollisionShape(std::string_view name, ModelWork* model, UINT type)
{
	RegisterCollisionShape(name, std::make_unique<ModelCollision>(std::shared_ptr<ModelWork>(model), type));
}

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

	bool isHit = shape.Intersects(target.m_pos, target.m_dir, hitDistance);

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

//=================================================================================================================================
// ModelCollision
// 3D���b�V���̌`��
//=================================================================================================================================

// ��vs���f���̏Փ˔���
bool ModelCollision::Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pRes)
{
	if (!m_isEnable || !m_shape) { return false; }

	std::shared_ptr<ModelData> spModelData = m_shape->GetModelData();
	if (!spModelData) { return false; }

	const std::vector<ModelData::Node>& dataNodes = spModelData->GetNodes();
	const std::vector<ModelWork::Node>& workNodes = m_shape->GetNodes();

	DirectX::BoundingSphere pushedSphere = target;
	Math::Vector3 pushedSphereCenter = DirectX::XMLoadFloat3(&pushedSphere.Center);

	bool isHit = false;

	Math::Vector3 hitPos;

	for (int idx : spModelData->GetCollisionMeshNodeIndices())
	{
		const ModelData::Node& dataNode = dataNodes[idx];
		const ModelWork::Node& workNode = workNodes[idx];

		if (!dataNode.spMesh) { continue; }

		CollisionMeshResult tmpResult;
		CollisionMeshResult* pTmpResult = pRes ? &tmpResult : nullptr;

		if (!MeshIntersect(*dataNode.spMesh, pushedSphere, workNode.mWorld * world, pTmpResult))
		{
			continue;
		}

		if (!pRes) { return true; }

		isHit = true;

		pushedSphereCenter = DirectX::XMVectorAdd(pushedSphereCenter, DirectX::XMVectorScale(tmpResult.hitDir, tmpResult.overlapDistance));

		DirectX::XMStoreFloat3(&pushedSphere.Center, pushedSphereCenter);

		hitPos = tmpResult.hitPos;
	}

	if (pRes && isHit)
	{
		pRes->m_hitPos = hitPos;

		pRes->m_hitDir = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&pushedSphere.Center), DirectX::XMLoadFloat3(&target.Center));
	
		pRes->m_overlapDistance = DirectX::XMVector3Length(pRes->m_hitDir).m128_f32[0];

		pRes->m_hitDir = DirectX::XMVector3Normalize(pRes->m_hitDir);
	}

	return isHit;
}

// ���Cvs���f���̏Փ˔���
bool ModelCollision::Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pRes)
{
	if (!m_isEnable || !m_shape) { return false; }

	std::shared_ptr<ModelData> spModelData = m_shape->GetModelData();
	if (!spModelData) { return false; }

	CollisionMeshResult nearestResult;

	bool isHit = false;

	const std::vector<ModelData::Node>& dataNodes = spModelData->GetNodes();
	const std::vector<ModelWork::Node>& workNodes = m_shape->GetNodes();

	for (int index : spModelData->GetCollisionMeshNodeIndices())
	{
		const ModelData::Node& dataNode = dataNodes[index];
		const ModelWork::Node& workNode = workNodes[index];

		if (!dataNode.spMesh) { continue; }

		CollisionMeshResult tmpResult;
		CollisionMeshResult* pTmpResult = pRes ? &tmpResult : nullptr;

		if (!MeshIntersect(*dataNode.spMesh, target.m_pos, target.m_dir, target.m_range,
			workNode.mWorld * world, pTmpResult))
		{
			continue;
		}

		// �ڍ׃��U���g���K�v������Α����ʂ�Ԃ�
		if (!pRes) { return true; }

		isHit = true;

		if (tmpResult.overlapDistance > nearestResult.overlapDistance)
		{
			nearestResult = tmpResult;
		}
	}

	if (pRes && isHit)
	{
		// �ł��߂��œ��������q�b�g�����R�s�[����
		pRes->m_hitPos = nearestResult.hitPos;

		pRes->m_hitDir = nearestResult.hitDir;

		pRes->m_overlapDistance = nearestResult.overlapDistance;
	}

	return isHit;
}
