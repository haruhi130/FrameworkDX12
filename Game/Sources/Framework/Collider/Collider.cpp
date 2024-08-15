#include "Collider.h"

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

	bool isHit = shape.Intersects(target.m_pos,target.m_dir,hitDistance);

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
