#pragma once

class BaseScene;

class SceneManager
{
public:
	enum class SceneType
	{
		Title,
		Game,
		//Result,
	};

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();

	void SetNextScene(SceneType nextScene)
	{
		m_nextSceneType = nextScene;
	}

private:

	void Init() { ChangeScene(m_currentSceneType); }

	void ChangeScene(SceneType type);

	std::shared_ptr<BaseScene> m_currentScene = nullptr;
	SceneType m_currentSceneType = SceneType::Game;
	SceneType m_nextSceneType = m_currentSceneType;

	SceneManager(){}
	~SceneManager(){}

public:
	static SceneManager& GetInstance()
	{
		static SceneManager instance;
		return instance;
	}
};