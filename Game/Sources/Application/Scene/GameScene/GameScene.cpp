#include "GameScene.h"
#include "../../GameObject/Camera/GameCamera.h"

void GameScene::Event()
{
}

void GameScene::Init()
{
	std::shared_ptr<GameCamera> camera = std::make_shared<GameCamera>();
	m_objList.push_back(camera);
}
