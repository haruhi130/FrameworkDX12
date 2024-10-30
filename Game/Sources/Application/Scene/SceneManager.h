#pragma once

class BaseScene;

class SceneManager
{
public:
	enum class SceneType
	{
		Title,
		Game,
		Clear,
		Failed,
	};

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();
	void DrawSprite();

	void ImGuiUpdate();

	void SetNextScene(SceneType nextScene)
	{
		m_nextSceneType = nextScene;
	}

	const std::list<std::shared_ptr<BaseObject>>& GetObjList();
	void AddObject(const std::shared_ptr<BaseObject>& obj);

	float GetBGMVolume() const
	{ return m_bgmVolume; }

	float GetSEVolume() const
	{ return m_seVolume; }

private:

	void Init() { ChangeScene(m_currentSceneType); }

	void ChangeScene(SceneType type);

	std::shared_ptr<BaseScene> m_currentScene = nullptr;
	SceneType m_currentSceneType = SceneType::Title;
	SceneType m_nextSceneType = m_currentSceneType;

	float m_bgmVolume = 15.0f;
	float m_seVolume = 15.0f;

	SceneManager() { Init(); }
	~SceneManager(){}

public:
	static SceneManager& GetInstance()
	{
		static SceneManager instance;
		return instance;
	}
};