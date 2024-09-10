#pragma once

class Mouse : public BaseObject
{
public:
	Mouse() { Init(); }
	~Mouse()override{}

	void Update()override;
	void PostUpdate()override;

	void Draw()override;

	void SetAnimeTime(float time) { animeTime = time; }

private:
	void Init()override;

	std::shared_ptr<ModelWork> m_spModel = nullptr;
	std::shared_ptr<Animator> m_spAnimator = nullptr;

	Math::Vector3 m_vec = {};

	float animeTime = 1.0f;

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

		void Enter(Mouse& owner)	override;
		void Update(Mouse& owner)	override;
		void Exit(Mouse& owner)	override;
	};

	// 歩き状態
	class ActionWalk : public ActionStateBase
	{
	public:
		~ActionWalk()override {}

		void Enter(Mouse& owner)	override;
		void Update(Mouse& owner)	override;
		void Exit(Mouse& owner)	override;
	};

	void ChangeActionState(std::shared_ptr<ActionStateBase> nextState);
	std::shared_ptr<ActionStateBase> m_currentAction = nullptr;
};