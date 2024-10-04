#include "SceneManager.h"

#include "BaseScene/BaseScene.h"
#include "GameScene/GameScene.h"
#include "TitleScene/TitleScene.h"
#include "ResultScene/ResultScene.h"

void SceneManager::PreUpdate()
{
	if (m_currentSceneType != m_nextSceneType)
	{
		ChangeScene(m_nextSceneType);
	}
	m_currentScene->PreUpdate();
}

void SceneManager::Update()
{
	m_currentScene->Update();
}

void SceneManager::PostUpdate()
{
	m_currentScene->PostUpdate();
}

void SceneManager::PreDraw()
{
	m_currentScene->PreDraw();
}

void SceneManager::Draw()
{
	m_currentScene->Draw();
}

void SceneManager::ImGuiUpdate()
{
	m_currentScene->ImGuiUpdate();
}

const std::list<std::shared_ptr<BaseObject>>& SceneManager::GetObjList()
{
	return m_currentScene->GetObjList();
}

void SceneManager::AddObject(const std::shared_ptr<BaseObject>& obj)
{
	m_currentScene->AddObject(obj);
}

void SceneManager::ChangeScene(SceneType type)
{
	switch (type)
	{
	case SceneType::Title:
		GraphicsDevice::GetInstance().SetBackBufferColor({ 0.8f,0.5f,0.3f,1.0f });
		m_currentScene = std::make_shared<TitleScene>();
		break;
	case SceneType::Game:
		GraphicsDevice::GetInstance().SetBackBufferColor({ 0.5f,0.5f,0.5f,1.0f });
		m_currentScene = std::make_shared<GameScene>();
		break;
	case SceneType::Result:
		GraphicsDevice::GetInstance().SetBackBufferColor({ 0.1f,0.3f,0.5f,1.0f });
		m_currentScene = std::make_shared<ResultScene>();
		break;
	}

	m_currentSceneType = type;
}
