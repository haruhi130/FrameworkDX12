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
	// 衝突判定が無効タイプの場合早期リターン
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

			// 詳細な衝突結果が必要ない場合は1つでも接触して返す
			if (!pResults) { break; }

			pResults->push_back(tmpRes);
		}
	}

	return isHit;
}

bool Collider::Intersects(const RayInfo& targetShape, const Math::Matrix& ownerMat, std::list<Collider::CollisionResult>* pResults) const
{
	// 衝突判定が無効タイプの場合早期リターン
	if (targetShape.m_type & m_disableType) { return false; }

	// レイの方向ベクトルが存在しない場合判定不能なので早期リターン
	if (!targetShape.m_dir.LengthSquared())
	{
		assert(0 && "Collider::Intersects:レイの方向ベクトルが存在しません");

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

			// 詳細な衝突結果が必要ない場合は1つでも接触して返す
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
// 球形の形状
//=================================================================================================================================

// 球vs球の衝突判定
bool SphereCollision::Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pRes)
{
	if (!m_isEnable) { return false; }

	DirectX::BoundingSphere shape;

	m_shape.Transform(shape, world);

	bool isHit = shape.Intersects(target);

	// 詳細が必要ない場合
	if (!pRes) { return isHit; }

	// 衝突時のみ計算
	if (isHit)
	{
		// 互いに当たらない最小距離
		float needDistance = target.Radius + shape.Radius;

		// 自身から相手に向かう方向ベクトル
		pRes->m_hitDir = (Math::Vector3(target.Center) - Math::Vector3(shape.Center));
		float betweenDistance = pRes->m_hitDir.Length();

		// 重なり量 = 互いに当たらない最小距離 - 互いの実際距離
		pRes->m_overlapDistance = needDistance - betweenDistance;

		pRes->m_hitDir.Normalize();

		// 互いの球の衝突の中心点
		pRes->m_hitPos = shape.Center + pRes->m_hitDir * (shape.Radius + pRes->m_overlapDistance * 0.5f);
	}

	return isHit;
}

// 球vsレイの衝突判定
bool SphereCollision::Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pRes)
{
	if (!m_isEnable) { return false; }

	DirectX::BoundingSphere shape;

	m_shape.Transform(shape, world);

	float hitDistance = 0.0f;

	bool isHit = shape.Intersects(target.m_pos, target.m_dir, hitDistance);

	// 判定限界距離を加味
	isHit &= (target.m_range >= hitDistance);

	// 詳細が必要ない場合
	if (!pRes) { return isHit; }

	// 衝突時のみ計算
	if (isHit)
	{
		// レイ発射位置 + レイの当たった位置までのベクトル	
		pRes->m_hitPos = target.m_pos + target.m_dir * hitDistance;

		pRes->m_hitDir = target.m_dir * (-1);

		pRes->m_overlapDistance = target.m_range - hitDistance;
	}

	return isHit;
}

//=================================================================================================================================
// ModelCollision
// 3Dメッシュの形状
//=================================================================================================================================

// 球vsモデルの衝突判定
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

// レイvsモデルの衝突判定
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

		// 詳細リザルトが必要無ければ即結果を返す
		if (!pRes) { return true; }

		isHit = true;

		if (tmpResult.overlapDistance > nearestResult.overlapDistance)
		{
			nearestResult = tmpResult;
		}
	}

	if (pRes && isHit)
	{
		// 最も近くで当たったヒット情報をコピーする
		pRes->m_hitPos = nearestResult.hitPos;

		pRes->m_hitDir = nearestResult.hitDir;

		pRes->m_overlapDistance = nearestResult.overlapDistance;
	}

	return isHit;
}
