#pragma once

class GameCamera;

class Mouse : public BaseObject
{
public:
	Mouse() { Init(); }
	~Mouse()override{}

	void Update()override;
	void PostUpdate()override;

	void Draw()override;

	// カメラ取得
	void SetCamera(const std::shared_ptr<GameCamera>& camera)
	{ m_wpCamera = camera; }

	// 衝突対象を登録
	void RegistHitObjList(const std::shared_ptr<BaseObject>& obj)
	{ m_wpHitObjList.push_back(obj); }

private:
	void Init()override;

	// 行列更新
	void UpdateMatrix();

	// 回転更新
	void UpdateRotate(Math::Vector3& moveVec);
	// 衝突判定更新
	void UpdateCollision();

	// モデル
	std::shared_ptr<ModelWork> m_spModel = nullptr;
	// アニメーター
	std::shared_ptr<Animator> m_spAnimator = nullptr;

	// 座標
	Math::Vector3 m_pos = {};
	// 回転情報格納
	Math::Vector3 m_rot = {};

	// 重力
	float m_gravity = 0.0f;
	
	// 接地判定
	bool m_isGround = false;

	// 衝突対象リスト
	std::list<std::weak_ptr<BaseObject>> m_wpHitObjList;

	// カメラ
	std::weak_ptr<GameCamera> m_wpCamera;

	/////////////////////////////////////////////////
	// ステートパターン管理
private:
	class ActionStateBase
	{
	public:
		virtual ~ActionStateBase(){}

		virtual void Enter(Mouse& owner) = 0;
		virtual void Update(Mouse& owner) = 0;
		virtual void Exit(Mouse& owner) = 0;
	};

	// 待機状態
	class ActionIdle : public ActionStateBase
	{
	public:
		~ActionIdle()override {}

		void Enter(Mouse& owner) override;
		void Update(Mouse& owner) override;
		void Exit(Mouse& owner)	override;
	};

	// 歩き状態
	class ActionWalk : public ActionStateBase
	{
	public:
		~ActionWalk()override {}

		void Enter(Mouse& owner) override;
		void Update(Mouse& owner) override;
		void Exit(Mouse& owner)	override;
	};

	void ChangeActionState(std::shared_ptr<ActionStateBase> nextState);
	std::shared_ptr<ActionStateBase> m_currentAction = nullptr;
};