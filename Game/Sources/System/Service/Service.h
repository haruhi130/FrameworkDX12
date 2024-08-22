#pragma once

class Service
{
public:
	virtual void Start(){}
	virtual void Update() = 0;
};

class ServiceLocator
{
public:
	// �T�[�r�X�̐V�K�o�^
	template<class type>
	static void Add(std::shared_ptr<type> service)
	{
		if (!service) { return; }

		// �^���𐔒l�f�[�^�ɕϊ�
		size_t classID = typeid(type).hash_code();

		// �N���XID���L�[�ɂ��ăT�[�r�X���o����
		m_services[classID] = service;
	}

	// �^�w�肵�ăT�[�r�X���擾
	template<class type>
	static std::shared_ptr<type> Get()
	{
		size_t classID = typeid(type).hash_code();

		if (m_services.find(classID) == m_services.end())
		{
			// �w�肳�ꂽ�T�[�r�X���Ȃ������ꍇ
			return nullptr;
		}

		// �w�肳�ꂽ�T�[�r�X���擾
		std::shared_ptr<Service> spService = m_services[classID];

		// �w�肳�ꂽ�^�Ƀ_�E���L���X�g
		std::shared_ptr<type> spDcService = std::static_pointer_cast<type>(spService);

		return spDcService;
	}

	// �T�[�r�X�̔j��
	template<class type>
	static void Remove()
	{
		auto it = m_services.find(typeid(type).hash_code());
		if (it == m_services.end()) { return; }
		m_services.erase(it);
	}

	static void Update()
	{
		for (auto&& sv : m_services)
		{
			sv.second->Update();
		}
	}

private:
	// �T�[�r�X�ꗗ���X�g
	static inline std::unordered_map<size_t, std::shared_ptr<Service>> m_services;

};