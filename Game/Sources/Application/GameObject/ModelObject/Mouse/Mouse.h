#pragma once

#include "../ModelObject.h"

class GameCamera;

class Mouse : public ModelObject
{
public:
	Mouse() { Init(); }
	~Mouse()override{}

	void Update()override;
	void PostUpdate()override;
	void Draw()override;

	// 衝突対象を登録
	void RegistHitObjList(const std::shared_ptr<ModelObject>& obj)
	{ m_wpObjList.push_back(obj); }

	// カメラ登録
	void SetCamera(const std::shared_ptr<GameCamera>& camera)
	{ m_wpCamera = camera; }

	void ImGuiUpdate()override;

private:
	void Init()override;

	void UpdateMatrix()override;
	void UpdateRotate(Math::Vector3& moveVec)override;
	void UpdateCollision()override;

	// カメラ
	std::weak_ptr<GameCamera> m_wpCamera;

	// Hit対象オブジェクトリスト
	std::list<std::weak_ptr<ModelObject>> m_wpObjList;

	// 移動速度
	float m_speed = 0.0f;

	// 回転情報格納
	Math::Vector3 m_rot = {};

	// 重力
	float m_gravity = 0.0f;

	// 接地判定
	bool m_isGround = false;

	float m_coolTime = 60.0f;
	bool m_isMasked = false;

	std::shared_ptr<ModelWork> m_maskedModel;

	std::shared_ptr<ModelWork> m_spOriginalModel;

/////////////////////////////////////////////////
// ステートパターン管理
private:
	class ActionStateBase
	{
	public:
		virtual ~ActionStateBase() {}

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