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
		if (service == nullptr) { return; }

		// �N���XID���L�[�ɂ��ăT�[�r�X���o����
		m_services[typeid(type).hash_code()] = service;
	}

	// �^�w�肵�ăT�[�r�X���擾
	template<class type>
	static std::shared_ptr<type> Get()
	{
		if (m_services.find(typeid(type).hash_code()) == m_services.end())
		{
			return nullptr;
		}

		return std::static_pointer_cast<type>(m_services[typeid(type).hash_code()]);
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