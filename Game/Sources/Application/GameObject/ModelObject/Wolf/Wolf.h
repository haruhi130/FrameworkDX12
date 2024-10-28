#pragma once

#include "../ModelObject.h"

class Wolf : public ModelObject
{
public:
	Wolf() { Init(); }
	~Wolf()override{}

	void Update()override;
	void PostUpdate()override;
	void Draw()override;

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

	float m_rotTime = 0.0f;

	int m_r = 1;

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