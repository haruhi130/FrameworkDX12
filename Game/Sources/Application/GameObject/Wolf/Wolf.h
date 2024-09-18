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

	bool m_isSight = false;

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