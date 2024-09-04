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
		// 指定されたコンポーネントをインスタンス化
		std::shared_ptr<Component> spComp = std::make_shared<type>();
		// コンポーネントリストに追加
		m_spComponents.push_back(spComp);
		// コンポーネント側に付与された事を通知
		spComp->Attach(shared_from_this());

		return shared_from_this();
	}

	// コンポーネント取得
	template<class type>
	std::shared_ptr<type> GetComponent()
	{
		for (auto&& comp : m_spComponents)
		{
			// ダウンキャストできるか試す
			std::shared_ptr<type> dcComp =
				std::dynamic_pointer_cast<type>(comp);
			if (dcComp != nullptr) { return dcComp; }
		}

		assert(0 && "指定されたコンポーネントがアタッチされていません");
		return nullptr;
	}

	Math::Matrix& Transform();
	std::shared_ptr<GameObject> GetParent();
	std::list <std::shared_ptr<GameObject>> GetChildren();

protected:
	// このGameObjectが持っているコンポーネントのリスト
	std::list<std::shared_ptr<Component>> m_spComponents;
};