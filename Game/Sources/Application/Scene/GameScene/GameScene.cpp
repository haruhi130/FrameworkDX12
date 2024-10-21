#include "GameScene.h"
#include "../SceneManager.h"

#include "../../GameObject/Camera/GameCamera.h"

#include "../../GameObject/ModelObject/Mouse/Mouse.h"
#include "../../GameObject/ModelObject/Stage/Stage.h"
#include "../../GameObject/ModelObject/Wolf/Wolf.h"
#include "../../GameObject/ModelObject/Sky/Sky.h"
#include "../../GameObject/ModelObject/Cheese/Cheese.h"
#include "../../GameObject/ModelObject/Pedestal/Pedestal.h"
#include "../../GameObject/SpriteObject/SpriteObject.h"

#include "../../GameObject/ModelObject/Box/Box.h"
#include "../../GameObject/ModelObject/Meat/Meat.h"
#include "../../GameObject/ModelObject/RiceBall/RiceBall.h"

void GameScene::Event()
{
	// シーン切り替え
	if (GetAsyncKeyState('O') & 0x8000)
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Result);
	}
	// シーン切り替え
	if (GetAsyncKeyState('L') & 0x8000)
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Failed);
	}
}

void GameScene::Init()
{
	ShowCursor(false);

	// カメラ
	std::shared_ptr<GameCamera> camera = std::make_shared<GameCamera>();
	m_objList.push_back(camera);

	// プレイヤー(ネズミ)
	std::shared_ptr<Mouse> mouse = std::make_shared<Mouse>();

	// 敵(オオカミ)
	std::shared_ptr<Wolf> wolf = std::make_shared<Wolf>();

	// 空
	std::shared_ptr<Sky> sky = std::make_shared<Sky>();
	m_objList.push_back(sky);

	// 地面
	std::shared_ptr<Stage> stage = std::make_shared<Stage>();
	camera->RegistHitObjList(stage);
	m_objList.push_back(stage);

	// 肉
	std::shared_ptr<Meat> meat = std::make_shared<Meat>();
	meat->SetPos({ 15,3.2f,0 });
	m_objList.push_back(meat);

	// おにぎり
	std::shared_ptr<RiceBall> rice = std::make_shared<RiceBall>();
	rice->SetPos({ -7,3.2f,10 });
	m_objList.push_back(rice);

	// 箱
	std::shared_ptr<Box> box = std::make_shared<Box>();
	box->SetPos({ -15,0,15 });
	m_objList.push_back(box);

	// チーズ(ゴール)
	std::shared_ptr<Cheese> cheese = std::make_shared<Cheese>();
	m_objList.push_back(cheese);

	// 台座(ゴール用)
	std::shared_ptr<Pedestal> pedestal = std::make_shared<Pedestal>();
	pedestal->SetPos({ 0,0,25 });
	mouse->RegistHitObjList(pedestal);
	wolf->RegistHitObjList(pedestal);
	m_objList.push_back(pedestal);

	// 肉用
	pedestal = std::make_shared<Pedestal>();
	pedestal->SetPos({ 15,0,0 });
	mouse->RegistHitObjList(pedestal);
	wolf->RegistHitObjList(pedestal);
	m_objList.push_back(pedestal);

	// おにぎり用
	pedestal = std::make_shared<Pedestal>();
	pedestal->SetPos({ -7,0,10 });
	mouse->RegistHitObjList(pedestal);
	wolf->RegistHitObjList(pedestal);
	m_objList.push_back(pedestal);

	wolf->RegistHitObjList(stage);
	wolf->RegistHitObjList(pedestal);
	wolf->RegistHitObjList(box);
	wolf->RegistHitObjList(meat);
	wolf->RegistHitObjList(rice);
	m_objList.push_back(wolf);

	// プレイヤーを対象に設定するもの
	sky->SetTarget(mouse);
	camera->SetTarget(mouse);
	wolf->RegistHitObjList(mouse);

	// プレイヤーが対象に設定するもの
	mouse->RegistHitObjList(stage);
	mouse->RegistHitObjList(cheese);
	mouse->RegistHitObjList(pedestal);
	mouse->RegistHitObjList(box);
	mouse->RegistHitObjList(meat);
	mouse->RegistHitObjList(rice);
	mouse->SetCamera(camera);

	m_objList.push_back(mouse);

	std::shared_ptr<SpriteObject> cursor = std::make_shared<SpriteObject>();
	cursor->SetPos({ 0,0 });
	cursor->SetRectangle({ 0,0,32,32 });
	cursor->SetTexture("Assets/Textures/mark.png");
	m_objList.push_back(cursor);

}
