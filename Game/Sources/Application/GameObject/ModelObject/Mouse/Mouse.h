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

	// �ՓˑΏۂ�o�^
	void RegistHitObjList(const std::shared_ptr<ModelObject>& obj)
	{ m_wpObjList.push_back(obj); }

	// �J�����o�^
	void SetCamera(const std::shared_ptr<GameCamera>& camera)
	{ m_wpCamera = camera; }

	void ImGuiUpdate()override;

private:
	void Init()override;

	void UpdateMatrix()override;
	void UpdateRotate(Math::Vector3& moveVec)override;
	void UpdateCollision()override;

	// �J����
	std::weak_ptr<GameCamera> m_wpCamera;

	// Hit�ΏۃI�u�W�F�N�g���X�g
	std::list<std::weak_ptr<ModelObject>> m_wpObjList;

	// �ړ����x
	float m_speed = 0.0f;

	// ��]���i�[
	Math::Vector3 m_rot = {};

	// �d��
	float m_gravity = 0.0f;

	// �ڒn����
	bool m_isGround = false;

	float m_coolTime = 60.0f;
	bool m_isMasked = false;

	std::shared_ptr<ModelWork> m_maskedModel;

	std::shared_ptr<ModelWork> m_spOriginalModel;

/////////////////////////////////////////////////
// �X�e�[�g�p�^�[���Ǘ�
private:
	class ActionStateBase
	{
	public:
		virtual ~ActionStateBase() {}

		virtual void Enter(Mouse& owner) = 0;
		virtual void Update(Mouse& owner) = 0;
		virtual void Exit(Mouse& owner) = 0;
	};

	// �ҋ@���
	class ActionIdle : public ActionStateBase
	{
	public:
		~ActionIdle()override {}

		void Enter(Mouse& owner) override;
		void Update(Mouse& owner) override;
		void Exit(Mouse& owner)	override;
	};

	// �������
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