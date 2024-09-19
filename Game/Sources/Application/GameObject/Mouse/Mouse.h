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

	// �J�����擾
	void SetCamera(const std::shared_ptr<GameCamera>& camera)
	{ m_wpCamera = camera; }

	// �ՓˑΏۂ�o�^
	void RegistHitObjList(const std::shared_ptr<BaseObject>& obj)
	{ m_wpHitObjList.push_back(obj); }

private:
	void Init()override;

	// �s��X�V
	void UpdateMatrix();

	// ��]�X�V
	void UpdateRotate(Math::Vector3& moveVec);
	// �Փ˔���X�V
	void UpdateCollision();

	// ���f��
	std::shared_ptr<ModelWork> m_spModel = nullptr;
	// �A�j���[�^�[
	std::shared_ptr<Animator> m_spAnimator = nullptr;

	// ���W
	Math::Vector3 m_pos = {};
	// ��]���i�[
	Math::Vector3 m_rot = {};

	// �d��
	float m_gravity = 0.0f;
	
	// �ڒn����
	bool m_isGround = false;

	// �ՓˑΏۃ��X�g
	std::list<std::weak_ptr<BaseObject>> m_wpHitObjList;

	// �J����
	std::weak_ptr<GameCamera> m_wpCamera;

	/////////////////////////////////////////////////
	// �X�e�[�g�p�^�[���Ǘ�
private:
	class ActionStateBase
	{
	public:
		virtual ~ActionStateBase(){}

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