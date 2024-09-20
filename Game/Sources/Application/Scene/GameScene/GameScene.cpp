#include "GameScene.h"
#include "../SceneManager.h"

#include "../../GameObject/Camera/GameCamera.h"
#include "../../GameObject/Mouse/Mouse.h"
#include "../../GameObject/Ground/Ground.h"
#include "../../GameObject/Wolf/Wolf.h"

void GameScene::Event()
{
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Title);
	}
}

void GameScene::Init()
{
	ShowCursor(false);

	std::shared_ptr<GameCamera> camera = std::make_shared<GameCamera>();
	m_objList.push_back(camera);

	std::shared_ptr<Ground> ground = std::make_shared<Ground>();
	camera->RegistHitObjList(ground);
	m_objList.push_back(ground);

	std::shared_ptr<Wolf> wolf = std::make_shared<Wolf>();
	wolf->RegistHitObjList(ground);
	m_objList.push_back(wolf);

	std::shared_ptr<Mouse> mouse = std::make_shared<Mouse>();
	camera->SetTarget(mouse);
	wolf->RegistHitObjList(mouse);
	mouse->RegistHitObjList(ground);
	mouse->SetCamera(camera);
	m_objList.push_back(mouse);
}
