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

	// �ՓˑΏۂ�o�^
	void RegistHitObjList(const std::shared_ptr<ModelObject>& obj)
	{ m_wpObjList.push_back(obj); }

	void SetPos(const Math::Vector3& pos)override
	{
		m_mWorld.Translation(pos);
	}

	void SetRotationY(float rotY)override
	{
		m_rot.y = rotY;
		m_mWorld *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(rotY));
	}

	void SetSightAngle(float angle)
	{ m_sightAngle = angle; }

	void SetMovePosition(const Math::Vector3& start, const Math::Vector3& end)
	{
		m_startPos = start;
		m_endPos = end;
		m_isMove = true;
		ChangeActionState(std::make_shared<ActionWalk>());
	}

	void SetSpeed(float speed)
	{
		m_speed = speed;
	}

	void SetTarget(const std::shared_ptr<ModelObject>& target)
	{
		if (!target)return;
		m_wpTarget = target;
	}

private:
	void Init()override;

	void UpdateMatrix()override;
	void UpdateRotate(Math::Vector3& moveVec)override;
	void UpdateCollision()override;

	// Hit�ΏۃI�u�W�F�N�g���X�g
	std::list<std::weak_ptr<ModelObject>> m_wpObjList;

	// �ڒn����
	bool m_isGround = false;

	// �d��
	float m_gravity = 0.0f;

	// ���F����
	float m_sightTime = 0.0f;

	// ���E������
	bool m_isSight = false;

	// ���E�p�x
	float m_sightAngle = 45.0f;

	// ��]���i�[
	Math::Vector3 m_rot = {};

	// ����
	Math::Vector3 m_startPos;
	Math::Vector3 m_endPos;

	float m_progress = 0.0f;
	float m_speed = 1.0f;

	bool m_isRevers = false;

	bool m_isMove = false;

	std::weak_ptr<ModelObject> m_wpTarget;

	std::shared_ptr<SoundInstance> m_spSound = nullptr;

/////////////////////////////////////////////////
// �X�e�[�g�p�^�[���Ǘ�
private:
	class ActionStateBase
	{
	public:
		virtual ~ActionStateBase() {}

		virtual void Enter(BullTank& owner) = 0;
		virtual void Update(BullTank& owner) = 0;
		virtual void Exit(BullTank& owner) = 0;
	};

	// �ҋ@���
	class ActionIdle : public ActionStateBase
	{
	public:
		~ActionIdle()override {}

		void Enter(BullTank& owner)	override;
		void Update(BullTank& owner) override;
		void Exit(BullTank& owner)	override;
	};

	// �������
	class ActionWalk : public ActionStateBase
	{
	public:
		~ActionWalk()override {}

		void Enter(BullTank& owner)	override;
		void Update(BullTank& owner) override;
		void Exit(BullTank& owner)	override;
	};

	// �������
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