#pragma once

class BaseObject : public std::enable_shared_from_this<BaseObject>
{
public:
	BaseObject(){}
	virtual ~BaseObject(){}

	virtual void Init(){}

	virtual void PreUpdate(){}
	virtual void Update(){}
	virtual void PostUpdate(){}

	virtual void PreDraw(){}
	virtual void Draw(){}

	virtual void ImGuiUpdate(){}

	virtual void SetPos(const Math::Vector3& pos)
	{ m_mWorld.Translation(pos); }

	virtual Math::Vector3 GetPos()const 
	{ return m_mWorld.Translation(); }

	const Math::Matrix& GetMatrix() const 
	{ return m_mWorld; }

	virtual bool IsExpired() const 
	{ return m_isExpired; }

	bool Intersects(const Collider::SphereInfo& target, std::list<Collider::CollisionResult>* pResults);
	bool Intersects(const Collider::RayInfo& target, std::list<Collider::CollisionResult>* pResults);

	virtual void OnHit(){}

protected:
	// カメラからの距離
	float m_distCamera = 0.0f;

	// 存在消滅フラグ
	bool m_isExpired = false;

	// 行列
	Math::Matrix m_mWorld;

	// 当たり判定クラス
	std::unique_ptr<Collider> m_upCollider = nullptr;
};