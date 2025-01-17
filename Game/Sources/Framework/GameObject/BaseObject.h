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
	virtual void DrawShadow(){}
	virtual void Draw(){}
	virtual void DrawSprite(){}

	virtual void ImGuiUpdate(){}

	// ��]�ݒ�
	virtual void SetRotationX(float rotX)
	{ m_mWorld *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(rotX)); }

	virtual void SetRotationY(float rotY)
	{ m_mWorld *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotY)); }

	virtual void SetRotationZ(float rotZ)
	{ m_mWorld *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(rotZ)); }
	
	// �X�P�[���ݒ�
	virtual void SetScale(float scale)
	{ m_mWorld *= Math::Matrix::CreateScale(scale); }
	
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
	Math::Matrix m_mWorld = Math::Matrix::Identity;

	// �����蔻��N���X
	std::unique_ptr<Collider> m_upCollider = nullptr;
};