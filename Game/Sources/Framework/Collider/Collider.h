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
		Sight	= 1 << 2, // ���E:�������������I�u�W�F�N�g
		Event	= 1 << 3, // �C�x���g:����Ȕ���
		Goal	= 1 << 4, // �S�[��:�N���A����
	};

	// ���`�̏Փ˔�����:�Փ˂��鑤
	struct SphereInfo
	{
		SphereInfo() {}

		// BoundingSphere�𒼐ڎw��
		SphereInfo(UINT type, const DirectX::BoundingSphere sphere)
			:m_type(type), m_sphere(sphere) {}

		// ���W�Ɣ��a����BoundingSphere���w��
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

		// ���C�̏���S�Ďw��
		RayInfo(UINT type, const Math::Vector3& pos, const Math::Vector3& dir, float range)
			:m_type(type), m_pos(pos), m_dir(dir), m_range(range) {}

		// �J�n�n�_�ƏI���n�_���烌�C�̏����쐬
		RayInfo(UINT type, const Math::Vector3& start, const Math::Vector3& end)
			:m_type(type), m_pos(start)
		{
			m_dir = end - start;
			m_range = m_dir.Length();
			m_dir.Normalize();
		}

		Math::Vector3 m_pos;	// ���C�̔��ˈʒu
		Math::Vector3 m_dir;	// ���C�̕���
		float m_range = 0.0f;	// ���C�̌��E����

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

	// �Փ˔���`��o�^
	void RegisterCollisionShape(std::string_view name, std::unique_ptr<CollisionShape> spShape);
	void RegisterCollisionShape(std::string_view name, const DirectX::BoundingSphere& sphere, UINT type);
	void RegisterCollisionShape(std::string_view name, const Math::Vector3& localPos, float radius, UINT type);
	void RegisterCollisionShape(std::string_view name, const std::shared_ptr<ModelData>& model, UINT type);
	void RegisterCollisionShape(std::string_view name, ModelData* model, UINT type);
	void RegisterCollisionShape(std::string_view name, const std::shared_ptr<ModelWork>& model, UINT type);
	void RegisterCollisionShape(std::string_view name, ModelWork* model, UINT type);

	// �Փ˔�����s
	bool Intersects(const SphereInfo& targetShape, const Math::Matrix& ownerMat, std::list<Collider::CollisionResult>* pResults)const;
	bool Intersects(const RayInfo& targetShape, const Math::Matrix& ownerMat, std::list<Collider::CollisionResult>* pResults)const;

	// �Փ˔���̗L��/�����̐ݒ�
	void SetEnable(std::string_view name, bool isEnable);
	void SetEnable(int type, bool isEnable);
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

	UINT GetType()const { return m_type; }

	virtual bool Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pResult) = 0;
	virtual bool Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pResult) = 0;

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

class ModelCollision :public CollisionShape
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