#include "GameScene.h"
#include "../SceneManager.h"

#include "../../GameObject/Camera/GameCamera.h"

#include "../../GameObject/ModelObject/Mouse/Mouse.h"
#include "../../GameObject/ModelObject/Stage/Stage.h"
#include "../../GameObject/ModelObject/Wolf/Wolf.h"
#include "../../GameObject/ModelObject/Sky/Sky.h"
#include "../../GameObject/ModelObject/Cheese/Cheese.h"
#include "../../GameObject/ModelObject/Pedestal/Pedestal.h"

#include "../../GameObject/Test/Test.h"

void GameScene::Event()
{
	// シーン切り替え
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Result);
	}
}

void GameScene::Init()
{
	//ShowCursor(false);

	std::shared_ptr<Test> test = std::make_shared<Test>();
	test->SetRectangle({ 0,0,1600,900 });
	test->SetPos({ 320,180 });
	m_objList.push_back(test);

	// カメラ
	std::shared_ptr<GameCamera> camera = std::make_shared<GameCamera>();
	m_objList.push_back(camera);

	// 空
	std::shared_ptr<Sky> sky = std::make_shared<Sky>();
	m_objList.push_back(sky);

	// 地面
	std::shared_ptr<Stage> stage = std::make_shared<Stage>();
	camera->RegistHitObjList(stage);
	m_objList.push_back(stage);

	// チーズ(ゴール)
	std::shared_ptr<Cheese> cheese = std::make_shared<Cheese>();
	m_objList.push_back(cheese);

	// 台座
	std::shared_ptr<Pedestal> pedestal = std::make_shared<Pedestal>();
	m_objList.push_back(pedestal);

	// 敵(オオカミ)
	std::shared_ptr<Wolf> wolf = std::make_shared<Wolf>();
	wolf->RegistHitObjList(stage);
	wolf->RegistHitObjList(pedestal);
	m_objList.push_back(wolf);

	// プレイヤー(ネズミ)
	std::shared_ptr<Mouse> mouse = std::make_shared<Mouse>();
	// プレイヤーを対象に設定するもの
	sky->SetTarget(mouse);
	camera->SetTarget(mouse);
	wolf->RegistHitObjList(mouse);

	// プレイヤーが対象に設定するもの
	mouse->RegistHitObjList(stage);
	mouse->RegistHitObjList(cheese);
	mouse->RegistHitObjList(pedestal);
	mouse->SetCamera(camera);
	
	m_objList.push_back(mouse);
}
