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

	// 衝突対象を登録
	void RegistHitObjList(const std::shared_ptr<ModelObject>& obj)
	{ m_wpObjList.push_back(obj); }

	void SetPos(const Math::Vector3& pos)override
	{
		m_mWorld.Translation(pos);
	}

	void SetRotationY(float rotY)override
	{
		m_rotateVec.y = rotY;
		m_mWorld *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotY));
	}

	void SetSightInfo(float range = 5.0f,float angle = 45.0f)
	{
		m_sightRange = range;
		m_sightAngle = angle; 
	}

	void SetMovePosition(const Math::Vector3& start, const Math::Vector3& end,float speed)
	{
		m_speed = speed;
		m_startPos = start;
		m_endPos = end;
		m_isMove = true;
		ChangeActionState(std::make_shared<ActionWalk>());
	}

	void SetSpeed(float speed)
	{
		m_speed = speed;
	}

private:
	void Init()override;

	void UpdateMatrix()override;
	void UpdateRotate(Math::Vector3& moveVec)override;
	void UpdateCollision()override;

	// Hit対象オブジェクトリスト
	std::list<std::weak_ptr<ModelObject>> m_wpObjList;

	// 接地判定
	bool m_isGround = false;

	// 重力
	float m_gravity = 0.0f;

	// 視界内判定
	bool m_isSight = false;
	// 視認時間
	float m_sightTime = 0.0f;
	// 視界範囲
	float m_sightRange = 5.0f;
	// 視界角度
	float m_sightAngle = 45.0f;

	// 回転情報
	Math::Vector3 m_rotateVec = {};

	// 移動用変数
	Math::Vector3 m_startPos;
	Math::Vector3 m_endPos;
	float m_progress = 0.0f;
	float m_speed = 1.0f;
	bool m_isRevers = false;
	bool m_isMove = false;

	// SE設定
	std::shared_ptr<SoundInstance> m_spSound = nullptr;

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

	// 発見状態
	class ActionSight : public ActionStateBase
	{
	public:
		~ActionSight()override {}

		void Enter(BullTank& owner)	override;
		void Update(BullTank& owner) override;
		void Exit(BullTank& owner)	override;
	};

	void ChangeActionState(std::shared_ptr<ActionStateBase> nextState);
	std::shared_ptr<ActionStateBase> m_currentAction = nullptr;
};