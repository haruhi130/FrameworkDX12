#include "GameScene.h"
#include "../../GameObject/Camera/GameCamera.h"
#include "../../GameObject/Mouse/Mouse.h"
#include "../../GameObject/Ground/Ground.h"

void GameScene::Event()
{
}

void GameScene::Init()
{
	ShowCursor(false);

	std::shared_ptr<Ground> ground = std::make_shared<Ground>();
	m_objList.push_back(ground);

	std::shared_ptr<Mouse> mouse = std::make_shared<Mouse>();
	mouse->RegistHitObjList(ground);
	m_objList.push_back(mouse);

	std::shared_ptr<GameCamera> camera = std::make_shared<GameCamera>();
	camera->SetTarget(mouse);
	camera->RegistHitObjList(ground);
	mouse->SetCamera(camera);
	m_objList.push_back(camera);

}
