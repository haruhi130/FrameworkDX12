#pragma once

class CollisionShape;

class Collider
{
public:
	// �Փ˃^�C�v�pBit�t���O
	enum Type
	{
		Ground	= 1 << 0, // �n�`:��ɏ���I�u�W�F�N�g
		Bump	= 1 << 1, // �Փ�:�������̏d�Ȃ��h�~
		Event	= 1 << 2, // �C�x���g:����Ȕ���
		Sight	= 1 << 3, // ���E:�������������I�u�W�F�N�g
		Goal	= 1 << 4, // �S�[��:�N���A����
		Clear	= 1 << 5,
	};

	// ���`�̏Փ˔�����:�Փ˂��鑤
	struct SphereInfo
	{
		SphereInfo() {}

		/// <summary>
		/// BoundingSphere�𒼐ڎw��
		/// </summary>
		/// <param name="type">�Փ˃^�C�v</param>
		/// <param name="sphere">�X�t�B�A���</param>
		SphereInfo(UINT type, const DirectX::BoundingSphere sphere)
			:m_type(type), m_sphere(sphere) {}

		/// <summary>
		/// ���W�Ɣ��a����BoundingSphere���w��
		/// </summary>
		/// <param name="type">�Փ˃^�C�v</param>
		/// <param name="pos">�X�t�B�A���W</param>
		/// <param name="radius">�X�t�B�A���a</param>
		SphereInfo(UINT type, const Math::Vector3& pos, float radius)
			:m_type(type)
		{
			m_sphere.Center = pos;
			m_sphere.Radius = radius;
		}

		DirectX::BoundingSphere m_sphere;

		UINT m_type = 0;
	};

	// ���C�̏Փ˔�����:�Փ˂��鑤
	struct RayInfo
	{
		RayInfo() {}

		/// <summary>
		/// ���C����S�Ďw�肵�č쐬
		/// </summary>
		/// <param name="type">�Փ˃^�C�v</param>
		/// <param name="pos">���C���W</param>
		/// <param name="dir">���C����</param>
		/// <param name="range">���C�̒���</param>
		RayInfo(UINT type, const Math::Vector3& pos, const Math::Vector3& dir, float range)
			:m_type(type), m_pos(pos), m_dir(dir), m_range(range) {}

		/// <summary>
		/// �J�n�n�_�ƏI���n�_���烌�C�̏����쐬
		/// </summary>
		/// <param name="type">�Փ˃^�C�v</param>
		/// <param name="start">���C�J�n�n�_</param>
		/// <param name="end">���C�I���n�_</param>
		RayInfo(UINT type, const Math::Vector3& start, const Math::Vector3& end)
			:m_type(type), m_pos(start)
		{
			m_dir = end - start;
			m_range = m_dir.Length();
			m_dir.Normalize();
		}

		Math::Vector3 m_pos;	// ���C�̔��ˈʒu
		Math::Vector3 m_dir;	// ���C�̕���
		float m_range = 0.0f;	// ���C�̋���

		UINT m_type = 0;
	};

	// �ڍׂȏՓˌ���
	struct CollisionResult
	{
		Math::Vector3 m_hitPos;			// �Փ˂������W
		Math::Vector3 m_hitDir;			// �Ώۂ���̕���
		float m_overlapDistance = 0.0f; // �d�Ȃ�����
	};

	Collider() {}
	~Collider() {}

	/// <summary>
	/// �Փ˔���`��o�^
	/// </summary>	
	// CollisionShape����`��o�^
	void RegisterCollisionShape(std::string_view name, std::unique_ptr<CollisionShape> upShape);
	// �X�t�B�A�����`��o�^
	void RegisterCollisionShape(std::string_view name, const DirectX::BoundingSphere& sphere, UINT type);
	// �X�t�B�A�����w�肵�Č`��o�^
	void RegisterCollisionShape(std::string_view name, const Math::Vector3& localPos, float radius, UINT type);
	// �S�̃��f�����̃X�}�[�g�|�C���^����`��o�^
	void RegisterCollisionShape(std::string_view name, const std::shared_ptr<ModelData>& model, UINT type);
	// �S�̃��f�����̐��|�C���^����`��o�^
	void RegisterCollisionShape(std::string_view name, ModelData* model, UINT type);
	// �P�ꃂ�f�����̃X�}�[�g�|�C���^����`��o�^
	void RegisterCollisionShape(std::string_view name, const std::shared_ptr<ModelWork>& model, UINT type);
	// �P�ꃂ�f�����̐��|�C���^����`��o�^
	void RegisterCollisionShape(std::string_view name, ModelWork* model, UINT type);

	/// <summary>
	/// �Փ˔�����s
	/// </summary>
	/// <param name="targetShape">���肷����</param>
	/// <param name="ownerMat">���肷��I�u�W�F�N�g�̍s��</param>
	/// <param name="pResults">�ڍ׏��i�[���X�g</param>
	/// <returns>�Փ˂��Ă�����true</returns>
	bool Intersects(const SphereInfo& targetShape, const Math::Matrix& ownerMat, std::list<Collider::CollisionResult>* pResults) const;
	bool Intersects(const RayInfo& targetShape, const Math::Matrix& ownerMat, std::list<Collider::CollisionResult>* pResults) const;

	/// <summary>
	/// �Փ˔���̗L����/������
	/// </summary>
	/// <param name="name">�o�^�������O</param>
	/// <param name="isEnable">����̗L����/������</param>
	void SetEnable(std::string_view name, bool isEnable);
	/// <param name="name">�Փ˔���^�C�v</param>
	/// <param name="name">����̗L����/������</param>
	void SetEnable(int type, bool isEnable);
	/// <summary>
	/// �S�Ă̏Փ˔���̗L����/������
	/// </summary>
	/// <param name="isEnable">����̗L����/������</param>
	void SetEnableAll(bool isEnable);

private:
	std::unordered_map<std::string, std::unique_ptr<CollisionShape>> m_collisionShapes;

	int m_disableType = 0;
};

class CollisionShape
{
public:
	CollisionShape(UINT type) { m_type = type; }

	virtual ~CollisionShape() {}

	/// <summary>
	///	�Փ˔���̃^�C�v�擾
	/// </summary>
	/// <returns>�Փ˔���̃^�C�v</returns>
	UINT GetType() const { return m_type; }

	/// <summary>
	/// �Փ˔���̌v�Z
	/// </summary>
	/// <param name="target">�Փ˔�����s���`��</param>
	/// <param name="world">�s��</param>
	/// <param name="pResult">�ڍ׏��̊i�[���X�g</param>
	/// <returns>�Փ˂��Ă�����true</returns>
	virtual bool Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pResult) = 0;
	virtual bool Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pResult) = 0;

	/// <summary>
	/// �Փ˔���̗L����/������
	/// </summary>
	/// <param name="isEnable">����̗L����/������</param>
	void SetEnable(bool isEnable) { m_isEnable = isEnable; }

protected:
	bool m_isEnable = true;

private:
	UINT m_type = 0;
};

class SphereCollision : public CollisionShape
{
public:
	SphereCollision(const DirectX::BoundingSphere& sphere, UINT type)
		:CollisionShape(type), m_shape(sphere) {}
	SphereCollision(const Math::Vector3& localPos, float radius, UINT type)
		:CollisionShape(type) {
		m_shape.Center = localPos; m_shape.Radius = radius;
	}

	~SphereCollision()override {}

	bool Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pRes)override;
	bool Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pRes)override;

private:
	DirectX::BoundingSphere m_shape;
};

class ModelCollision : public CollisionShape
{
public:
	ModelCollision(const std::shared_ptr<ModelData>& model, UINT type)
		:CollisionShape(type), m_shape(std::make_shared<ModelWork>(model)) {}
	ModelCollision(const std::shared_ptr<ModelWork>& model, UINT type)
		:CollisionShape(type), m_shape(model) {}

	~ModelCollision()override{}
	
	bool Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pRes)override;
	bool Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pRes)override;

private:
	std::shared_ptr<ModelWork> m_shape;
};