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

private:
	void Init()override;

	void UpdateMatrix()override;
	void UpdateRotate(Math::Vector3& moveVec)override;
	void UpdateCollision()override;

	// Hit�ΏۃI�u�W�F�N�g���X�g
	std::list<std::weak_ptr<ModelObject>> m_wpObjList;

	// ���E������
	bool m_isSight = false;

	// ���E�p�x
	float m_sightAngle = 45.0f;

	// �d��
	float m_gravity = 0.0f;

	// �ڒn����
	bool m_isGround = false;

	float m_sightTime = 0.0f;

	// ��]���i�[
	Math::Vector3 m_rot = {};

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

	void ChangeActionState(std::shared_ptr<ActionStateBase> nextState);
	std::shared_ptr<ActionStateBase> m_currentAction = nullptr;
};