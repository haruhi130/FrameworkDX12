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

	/// <summary>
	/// ��]�s����Z�b�g
	/// </summary>
	/// <param name="rotX">X����]�p�x</param>
	virtual void SetRotationX(float rotX)
	{ m_mWorld *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(rotX)); }
	/// <param name="rotY">Y����]�p�x</param>
	virtual void SetRotationY(float rotY)
	{ m_mWorld *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotY)); }
	/// <param name="rotZ">Z����]�p�x</param>
	virtual void SetRotationZ(float rotZ)
	{ m_mWorld *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(rotZ)); }
	
	/// <summary>
	/// �g�k�s����Z�b�g
	/// </summary>
	/// <param name="scale">�g�k���l</param>
	virtual void SetScale(float scale)
	{ m_mWorld *= Math::Matrix::CreateScale(scale); }

	virtual void SetScale(const Math::Vector3& scale)
	{ m_mWorld *= Math::Matrix::CreateScale(scale); }
	
	/// <summary>
	/// ���W�s����Z�b�g
	/// </summary>
	/// <param name="pos">���W</param>
	virtual void SetPos(const Math::Vector3& pos)
	{ m_mWorld.Translation(pos); }

	/// <summary>
	/// �s�񂩂���W���擾
	/// </summary>
	/// <returns>���W</returns>
	inline virtual Math::Vector3 GetPos() const 
	{ return m_mWorld.Translation(); }

	/// <summary>
	/// �s����擾
	/// </summary>
	/// <returns>�s��</returns>
	inline const Math::Matrix& GetMatrix() const 
	{ return m_mWorld; }

	/// <summary>
	/// �I�u�W�F�N�g�����݂��Ă��邩����
	/// </summary>
	/// <returns>���݂��Ă���Ȃ�true</returns>
	inline virtual bool IsExpired() const 
	{ return m_isExpired; }

	/// <summary>
	/// �Փ˔�����s
	/// </summary>
	/// <param name="target">������s�����</param>
	/// <param name="pResults">�ڍ׏��i�[���X�g</param>
	/// <returns>�Փ˂��Ă�����true</returns>
	bool Intersects(const Collider::SphereInfo& target, std::list<Collider::CollisionResult>* pResults);
	bool Intersects(const Collider::RayInfo& target, std::list<Collider::CollisionResult>* pResults);

	virtual void OnHit(){}

protected:
	// �J��������̋���
	float m_distCamera = 0.0f;

	// ���ݏ��Ńt���O
	bool m_isExpired = false;

	Math::Matrix m_mWorld = Math::Matrix::Identity;

	// �����蔻��
	std::unique_ptr<Collider> m_upCollider = nullptr;
};