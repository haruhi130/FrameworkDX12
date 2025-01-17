#pragma once

#include "../ModelObject.h"

class BullTank : public ModelObject
{
public:
	BullTank() { Init(); }
	~BullTank()override{}

	void Update()override;
	void PostUpdate()override;

	void OnHit(bool isHit);

	void ImGuiUpdate()override;

	// 衝突対象を登録
	void RegistHitObjList(const std::shared_ptr<ModelObject>& obj)
	{ m_wpObjList.push_back(obj); }

private:
	void Init()override;

	void UpdateMatrix()override;
	void UpdateRotate(Math::Vector3& moveVec)override;
	void UpdateCollision()override;

	// Hit対象オブジェクトリスト
	std::list<std::weak_ptr<ModelObject>> m_wpObjList;

	// 視界内判定
	bool m_isSight = false;

	// 視界角度
	float m_sightAngle = 45.0f;

	// 重力
	float m_gravity = 0.0f;

	// 接地判定
	bool m_isGround = false;

	float m_sightTime = 0.0f;

	// 回転情報格納
	Math::Vector3 m_rot = {};

/////////////////////////////////////////////////
// ステートパターン管理
private:
	class ActionStateBase
	{
	public:
		virtual ~ActionStateBase() {}

		virtual void Enter(BullTank& owner) = 0;
		virtual void Update(BullTank& owner) = 0;
		virtual void Exit(BullTank& owner) = 0;
	};

	// 待機状態
	class ActionIdle : public ActionStateBase
	{
	public:
		~ActionIdle()override {}

		void Enter(BullTank& owner)	override;
		void Update(BullTank& owner) override;
		void Exit(BullTank& owner)	override;
	};

	// 歩き状態
	class ActionWalk : public ActionStateBase
	{
	public:
		~ActionWalk()override {}

		void Enter(BullTank& owner)	override;
		void Update(BullTank& owner) override;
		void Exit(BullTank& owner)	override;
	};

	void ChangeActionState(std::shared_ptr<ActionStateBase> nextState);
	std::shared_ptr<ActionStateBase> m_currentAction = nullptr;
};