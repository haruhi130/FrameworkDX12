#include "SceneManager.h"

#include "BaseScene/BaseScene.h"
#include "GameScene/GameScene.h"
#include "TitleScene/TitleScene.h"
#include "ResultScene/ResultScene.h"
#include "FailedResultScene/FailedResultScene.h"

void SceneManager::PreUpdate()
{
	if (m_isLoading) { return; }
	if (m_currentSceneType != m_nextSceneType)
	{
		ChangeScene(m_nextSceneType);
	}
	m_currentScene->PreUpdate();
}

void SceneManager::Update()
{
	if (m_isLoading) { return; }
	m_currentScene->Update();
}

void SceneManager::PostUpdate()
{
	if (m_isLoading) { return; }
	m_currentScene->PostUpdate();
}

void SceneManager::PreDraw()
{
	if (m_isLoading) { return; }
	m_currentScene->PreDraw();
}

void SceneManager::Draw()
{
	if (m_isLoading) { return; }
	m_currentScene->Draw();
}

void SceneManager::DrawSprite()
{
	m_currentScene->DrawSprite();
}

void SceneManager::ImGuiUpdate()
{
	if (m_isLoading) { return; }
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
	SetIsLoading(true);
	switch (type)
	{
	case SceneType::Title:
		m_currentScene = std::make_shared<TitleScene>();
		break;
	case SceneType::Game:
		m_currentScene = std::make_shared<GameScene>();
		break;
	case SceneType::Clear:
		m_currentScene = std::make_shared<ResultScene>();
		break;
	case SceneType::Failed:
		m_currentScene = std::make_shared<FailedResultScene>();
		break;
	}

	m_currentSceneType = type;
}
