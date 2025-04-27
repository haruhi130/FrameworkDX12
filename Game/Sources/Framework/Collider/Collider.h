#pragma once

class CollisionShape;

class Collider
{
public:
	// 衝突タイプ用Bitフラグ
	enum Type
	{
		Ground	= 1 << 0, // 地形:上に乗れるオブジェクト
		Bump	= 1 << 1, // 衝突:横方向の重なりを防止
		Event	= 1 << 2, // イベント:特殊な判定
		Sight	= 1 << 3, // 視界:視線判定をするオブジェクト
		Goal	= 1 << 4, // ゴール:クリア判定
		Clear	= 1 << 5,
	};

	// 球形の衝突判定情報:衝突する側
	struct SphereInfo
	{
		SphereInfo() {}

		/// <summary>
		/// BoundingSphereを直接指定
		/// </summary>
		/// <param name="type">衝突タイプ</param>
		/// <param name="sphere">スフィア情報</param>
		SphereInfo(UINT type, const DirectX::BoundingSphere sphere)
			:m_type(type), m_sphere(sphere) {}

		/// <summary>
		/// 座標と半径からBoundingSphereを指定
		/// </summary>
		/// <param name="type">衝突タイプ</param>
		/// <param name="pos">スフィア座標</param>
		/// <param name="radius">スフィア半径</param>
		SphereInfo(UINT type, const Math::Vector3& pos, float radius)
			:m_type(type)
		{
			m_sphere.Center = pos;
			m_sphere.Radius = radius;
		}

		DirectX::BoundingSphere m_sphere;

		UINT m_type = 0;
	};

	// レイの衝突判定情報:衝突する側
	struct RayInfo
	{
		RayInfo() {}

		/// <summary>
		/// レイ情報を全て指定して作成
		/// </summary>
		/// <param name="type">衝突タイプ</param>
		/// <param name="pos">レイ座標</param>
		/// <param name="dir">レイ方向</param>
		/// <param name="range">レイの長さ</param>
		RayInfo(UINT type, const Math::Vector3& pos, const Math::Vector3& dir, float range)
			:m_type(type), m_pos(pos), m_dir(dir), m_range(range) {}

		/// <summary>
		/// 開始地点と終了地点からレイの情報を作成
		/// </summary>
		/// <param name="type">衝突タイプ</param>
		/// <param name="start">レイ開始地点</param>
		/// <param name="end">レイ終了地点</param>
		RayInfo(UINT type, const Math::Vector3& start, const Math::Vector3& end)
			:m_type(type), m_pos(start)
		{
			m_dir = end - start;
			m_range = m_dir.Length();
			m_dir.Normalize();
		}

		Math::Vector3 m_pos;	// レイの発射位置
		Math::Vector3 m_dir;	// レイの方向
		float m_range = 0.0f;	// レイの距離

		UINT m_type = 0;
	};

	// 詳細な衝突結果
	struct CollisionResult
	{
		Math::Vector3 m_hitPos;			// 衝突した座標
		Math::Vector3 m_hitDir;			// 対象からの方向
		float m_overlapDistance = 0.0f; // 重なった量
	};

	Collider() {}
	~Collider() {}

	/// <summary>
	/// 衝突判定形状登録
	/// </summary>	
	// CollisionShapeから形状登録
	void RegisterCollisionShape(std::string_view name, std::unique_ptr<CollisionShape> upShape);
	// スフィア情報を形状登録
	void RegisterCollisionShape(std::string_view name, const DirectX::BoundingSphere& sphere, UINT type);
	// スフィア情報を指定して形状登録
	void RegisterCollisionShape(std::string_view name, const Math::Vector3& localPos, float radius, UINT type);
	// 全体モデル情報のスマートポインタから形状登録
	void RegisterCollisionShape(std::string_view name, const std::shared_ptr<ModelData>& model, UINT type);
	// 全体モデル情報の生ポインタから形状登録
	void RegisterCollisionShape(std::string_view name, ModelData* model, UINT type);
	// 単一モデル情報のスマートポインタから形状登録
	void RegisterCollisionShape(std::string_view name, const std::shared_ptr<ModelWork>& model, UINT type);
	// 単一モデル情報の生ポインタから形状登録
	void RegisterCollisionShape(std::string_view name, ModelWork* model, UINT type);

	/// <summary>
	/// 衝突判定実行
	/// </summary>
	/// <param name="targetShape">判定する情報</param>
	/// <param name="ownerMat">判定するオブジェクトの行列</param>
	/// <param name="pResults">詳細情報格納リスト</param>
	/// <returns>衝突していたらtrue</returns>
	bool Intersects(const SphereInfo& targetShape, const Math::Matrix& ownerMat, std::list<Collider::CollisionResult>* pResults) const;
	bool Intersects(const RayInfo& targetShape, const Math::Matrix& ownerMat, std::list<Collider::CollisionResult>* pResults) const;

	/// <summary>
	/// 衝突判定の有効化/無効化
	/// </summary>
	/// <param name="name">登録した名前</param>
	/// <param name="isEnable">判定の有効化/無効化</param>
	void SetEnable(std::string_view name, bool isEnable);
	/// <param name="name">衝突判定タイプ</param>
	/// <param name="name">判定の有効化/無効化</param>
	void SetEnable(int type, bool isEnable);
	/// <summary>
	/// 全ての衝突判定の有効化/無効化
	/// </summary>
	/// <param name="isEnable">判定の有効化/無効化</param>
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
	///	衝突判定のタイプ取得
	/// </summary>
	/// <returns>衝突判定のタイプ</returns>
	UINT GetType() const { return m_type; }

	/// <summary>
	/// 衝突判定の計算
	/// </summary>
	/// <param name="target">衝突判定を行う形状</param>
	/// <param name="world">行列</param>
	/// <param name="pResult">詳細情報の格納リスト</param>
	/// <returns>衝突していたらtrue</returns>
	virtual bool Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, Collider::CollisionResult* pResult) = 0;
	virtual bool Intersects(const Collider::RayInfo& target, const Math::Matrix& world, Collider::CollisionResult* pResult) = 0;

	/// <summary>
	/// 衝突判定の有効化/無効化
	/// </summary>
	/// <param name="isEnable">判定の有効化/無効化</param>
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