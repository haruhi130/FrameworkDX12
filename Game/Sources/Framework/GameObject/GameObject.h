#pragma once
class Component;

class GameObject final : public std::enable_shared_from_this<GameObject>
{
public:
	void Init();

	void Start(){}
	void Update();
	void Draw();

	template<class type>
	std::shared_ptr<GameObject> AddComponent()
	{
		// �w�肳�ꂽ�R���|�[�l���g���C���X�^���X��
		std::shared_ptr<Component> spComp = std::make_shared<type>();
		// �R���|�[�l���g���X�g�ɒǉ�
		m_spComponents.push_back(spComp);
		// �R���|�[�l���g���ɕt�^���ꂽ����ʒm
		spComp->Attach(shared_from_this());

		return shared_from_this();
	}

	// �R���|�[�l���g�擾
	template<class type>
	std::shared_ptr<type> GetComponent()
	{
		for (auto&& comp : m_spComponents)
		{
			// �_�E���L���X�g�ł��邩����
			std::shared_ptr<type> dcComp =
				std::dynamic_pointer_cast<type>(comp);
			if (dcComp != nullptr) { return dcComp; }
		}

		assert(0 && "�w�肳�ꂽ�R���|�[�l���g���A�^�b�`����Ă��܂���");
		return nullptr;
	}

	Math::Matrix& Transform();
	std::shared_ptr<GameObject> GetParent();
	std::list <std::shared_ptr<GameObject>> GetChildren();

protected:
	// ����GameObject�������Ă���R���|�[�l���g�̃��X�g
	std::list<std::shared_ptr<Component>> m_spComponents;
};