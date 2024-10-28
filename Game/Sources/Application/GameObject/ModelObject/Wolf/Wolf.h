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

	float m_rotTime = 0.0f;

	int m_r = 1;

/////////////////////////////////////////////////
// �X�e�[�g�p�^�[���Ǘ�
private:
	class ActionStateBase
	{
	public:
		virtual ~ActionStateBase() {}

		virtual void Enter(Wolf& owner) = 0;
		virtual void Update(Wolf& owner) = 0;
		virtual void Exit(Wolf& owner) = 0;
	};

	// �ҋ@���
	class ActionIdle : public ActionStateBase
	{
	public:
		~ActionIdle()override {}

		void Enter(Wolf& owner)	override;
		void Update(Wolf& owner) override;
		void Exit(Wolf& owner)	override;
	};

	// �������
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