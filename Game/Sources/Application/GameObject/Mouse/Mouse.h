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

	void SetAnimeTime(float time) { animeTime = time; }

	void SetCamera(const std::shared_ptr<GameCamera>& camera)
	{ m_wpCamera = camera; }

	void RegistHitObjList(const std::shared_ptr<BaseObject>& obj)
	{
		m_wpHitObjList.push_back(obj);
	}

private:
	void Init()override;

	void UpdateRotate(Math::Vector3& moveVec);
	void UpdateCollision();

	std::shared_ptr<ModelWork> m_spModel = nullptr;
	std::shared_ptr<Animator> m_spAnimator = nullptr;

	Math::Vector3 m_vec = {};
	Math::Vector3 m_rot = {};

	float m_gravity = 0.0f;
	bool m_isGround = false;
	std::list<std::weak_ptr<BaseObject>> m_wpHitObjList;

	float animeTime = 1.0f;

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