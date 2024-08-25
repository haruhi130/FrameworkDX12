#pragma once

class GameObject;

class Component
{
public:
	virtual ~Component(){}

	virtual void Attach(std::shared_ptr<GameObject> owner)
	{
		m_wpOwner = owner;
	}

	virtual void Update(){}
	virtual void Draw(){}

protected:
	std::weak_ptr<GameObject> m_wpOwner;
};

// GameObject�̈ʒu�����]�����Ǘ�����
// �܂��AGameObject���m�̐e�q�\�����Ǘ�����
// ���̃R���|�[�l���g��GameObject�̃��X�g�擪��
// �K���t�^����Ă�����̂Ƃ���
class TransformComponent :public Component
{
public:
	Math::Matrix& Transform()
	{
		return m_mTransform;
	}

	std::shared_ptr<GameObject> Parent()
	{
		return m_wpParent.lock();
	}

	std::list<std::shared_ptr<GameObject>>& Children()
	{
		return m_spChildren;
	}

private:
	// �ʒu��p��
	Math::Matrix m_mTransform;
	// �e��GameObject
	std::weak_ptr<GameObject> m_wpParent;
	// �q�B��GameObject
	std::list<std::shared_ptr<GameObject>> m_spChildren;
};

// ���f���Ǘ��p�R���|�[�l���g
class ModelComponent :public Component
{
public:
	~ModelComponent() {}

	void Update()override;
	void Draw()override;

	void SetModel(std::shared_ptr<ModelData> model);

private:
	std::shared_ptr<ModelWork> m_spModelWork = nullptr;

};