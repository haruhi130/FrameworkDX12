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
	virtual void DrawSprite(){}

	virtual void ImGuiUpdate(){}

	// ���W�ݒ�
	virtual void SetPos(const Math::Vector3& pos)
	{ m_mWorld.Translation(pos); }

	// ���W�擾
	inline virtual Math::Vector3 GetPos() const 
	{ return m_mWorld.Translation(); }

	// �s��擾
	inline const Math::Matrix& GetMatrix() const 
	{ return m_mWorld; }

	// �I�u�W�F�N�g���L����
	inline virtual bool IsExpired() const 
	{ return m_isExpired; }

	// �����蔻��
	bool Intersects(const Collider::SphereInfo& target, std::list<Collider::CollisionResult>* pResults);
	bool Intersects(const Collider::RayInfo& target, std::list<Collider::CollisionResult>* pResults);

	virtual void OnHit(){}

protected:
	// �J��������̋���
	float m_distCamera = 0.0f;

	// ���ݏ��Ńt���O
	bool m_isExpired = false;

	// �s��
	Math::Matrix m_mWorld;

	// �����蔻��N���X
	std::unique_ptr<Collider> m_upCollider = nullptr;
};