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
	// サービスの新規登録
	template<class type>
	static void Add(std::shared_ptr<type> service)
	{
		if (!service) { return; }

		// 型名を数値データに変換
		size_t classID = typeid(type).hash_code();

		// クラスIDをキーにしてサービスを覚える
		m_services[classID] = service;
	}

	// 型指定してサービスを取得
	template<class type>
	static std::shared_ptr<type> Get()
	{
		size_t classID = typeid(type).hash_code();

		if (m_services.find(classID) == m_services.end())
		{
			// 指定されたサービスがなかった場合
			return nullptr;
		}

		// 指定されたサービスを取得
		std::shared_ptr<Service> spService = m_services[classID];

		// 指定された型にダウンキャスト
		std::shared_ptr<type> spDcService = std::static_pointer_cast<type>(spService);

		return spDcService;
	}

	// サービスの破棄
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
	// サービス一覧リスト
	static inline std::unordered_map<size_t, std::shared_ptr<Service>> m_services;

};