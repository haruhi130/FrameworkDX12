#pragma once

class Wolf : public BaseObject
{
public:
	Wolf() { Init(); }
	~Wolf()override{}

	void Update()override;
	void PostUpdate()override;

	void Draw()override;

	void OnHit()override;
	
	// 衝突対象を登録
	void RegistHitObjList(const std::shared_ptr<BaseObject>& obj)
	{
		m_wpHitObjList.push_back(obj);
	}

	void ImGuiUpdate()override;

private:
	void Init()override;

	void UpdateCollision();

	// 衝突対象リスト
	std::list<std::weak_ptr<BaseObject>> m_wpHitObjList;

	std::shared_ptr<ModelWork> m_spModel = nullptr;
	std::shared_ptr<Animator> m_spAnimator = nullptr;

	// 視界内か
	bool m_isSight = false;

	// 視界内角
	float m_sightAngle = 45.0f;

	Math::Vector3 m_vec;

	// 1.視界の球判定の中 : OK
	// 2.正面方向から内角分までの左右内 : OK
	// 3.遮る物がないか判定 : NO
	// 4.OnHit(); : OK

	/////////////////////////////////////////////////
	// ステートパターン管理
private:
	class ActionStateBase
	{
	public:
		virtual ~ActionStateBase() {}

		virtual void Enter(Wolf& owner) = 0;
		virtual void Update(Wolf& owner) = 0;
		virtual void Exit(Wolf& owner) = 0;
	};

	// 待機状態
	class ActionIdle : public ActionStateBase
	{
	public:
		~ActionIdle()override {}

		void Enter(Wolf& owner)	override;
		void Update(Wolf& owner) override;
		void Exit(Wolf& owner)	override;
	};

	// 歩き状態
	class ActionWalk : public ActionStateBase
	{
	public:
		~ActionWalk()override {}

		void Enter(Wolf& owner)	override;
		void Update(Wolf& owner) override;
		void Exit(Wolf& owner)	override;
	};

	void ChangeActionState(std::shared_ptr<ActionStateBase> nextState);
	std::shared_ptr<ActionStateBase> m_currentAction = nullptr;
};