#pragma once

class Wolf : public BaseObject
{
public:
	Wolf() { Init(); }
	~Wolf()override{}

	void Update()override;
	void PostUpdate()override;

	void Draw()override;

	void OnHit()override;
	
	// �ՓˑΏۂ�o�^
	void RegistHitObjList(const std::shared_ptr<BaseObject>& obj)
	{
		m_wpHitObjList.push_back(obj);
	}

	void ImGuiUpdate()override;

private:
	void Init()override;

	void UpdateCollision();

	// �ՓˑΏۃ��X�g
	std::list<std::weak_ptr<BaseObject>> m_wpHitObjList;

	std::shared_ptr<ModelWork> m_spModel = nullptr;
	std::shared_ptr<Animator> m_spAnimator = nullptr;

	// ���E����
	bool m_isSight = false;

	// ���E���p
	float m_sightAngle = 45.0f;

	Math::Vector3 m_vec;

	// 1.���E�̋�����̒� : OK
	// 2.���ʕ���������p���܂ł̍��E�� : OK
	// 3.�Ղ镨���Ȃ������� : NO
	// 4.OnHit(); : OK

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