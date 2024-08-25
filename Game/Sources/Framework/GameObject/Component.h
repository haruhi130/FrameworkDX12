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

// GameObjectの位置情報や回転情報を管理する
// また、GameObject同士の親子構造を管理する
// このコンポーネントはGameObjectのリスト先頭に
// 必ず付与されているものとする
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
	// 位置や姿勢
	Math::Matrix m_mTransform;
	// 親のGameObject
	std::weak_ptr<GameObject> m_wpParent;
	// 子達のGameObject
	std::list<std::shared_ptr<GameObject>> m_spChildren;
};

// モデル管理用コンポーネント
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