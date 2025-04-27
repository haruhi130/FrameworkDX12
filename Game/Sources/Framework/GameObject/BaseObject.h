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
	/// 回転行列をセット
	/// </summary>
	/// <param name="rotX">X軸回転角度</param>
	virtual void SetRotationX(float rotX)
	{ m_mWorld *= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(rotX)); }
	/// <param name="rotY">Y軸回転角度</param>
	virtual void SetRotationY(float rotY)
	{ m_mWorld *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotY)); }
	/// <param name="rotZ">Z軸回転角度</param>
	virtual void SetRotationZ(float rotZ)
	{ m_mWorld *= Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(rotZ)); }
	
	/// <summary>
	/// 拡縮行列をセット
	/// </summary>
	/// <param name="scale">拡縮数値</param>
	virtual void SetScale(float scale)
	{ m_mWorld *= Math::Matrix::CreateScale(scale); }

	virtual void SetScale(const Math::Vector3& scale)
	{ m_mWorld *= Math::Matrix::CreateScale(scale); }
	
	/// <summary>
	/// 座標行列をセット
	/// </summary>
	/// <param name="pos">座標</param>
	virtual void SetPos(const Math::Vector3& pos)
	{ m_mWorld.Translation(pos); }

	/// <summary>
	/// 行列から座標を取得
	/// </summary>
	/// <returns>座標</returns>
	inline virtual Math::Vector3 GetPos() const 
	{ return m_mWorld.Translation(); }

	/// <summary>
	/// 行列を取得
	/// </summary>
	/// <returns>行列</returns>
	inline const Math::Matrix& GetMatrix() const 
	{ return m_mWorld; }

	/// <summary>
	/// オブジェクトが存在しているか判別
	/// </summary>
	/// <returns>存在しているならtrue</returns>
	inline virtual bool IsExpired() const 
	{ return m_isExpired; }

	/// <summary>
	/// 衝突判定実行
	/// </summary>
	/// <param name="target">判定を行う情報</param>
	/// <param name="pResults">詳細情報格納リスト</param>
	/// <returns>衝突していたらtrue</returns>
	bool Intersects(const Collider::SphereInfo& target, std::list<Collider::CollisionResult>* pResults);
	bool Intersects(const Collider::RayInfo& target, std::list<Collider::CollisionResult>* pResults);

	virtual void OnHit(){}

protected:
	// カメラからの距離
	float m_distCamera = 0.0f;

	// 存在消滅フラグ
	bool m_isExpired = false;

	Math::Matrix m_mWorld = Math::Matrix::Identity;

	// 当たり判定
	std::unique_ptr<Collider> m_upCollider = nullptr;
};