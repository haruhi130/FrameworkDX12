#include "GameScene.h"
#include "../SceneManager.h"

#include "../../GameObject/SkySphere/SkySphere.h"
#include "../../GameObject/Camera/GameCamera.h"
#include "../../GameObject/Mouse/Mouse.h"
#include "../../GameObject/Ground/Ground.h"
#include "../../GameObject/Wolf/Wolf.h"
#include "../../GameObject/Cheese/Cheese.h"

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

	// カメラ
	std::shared_ptr<GameCamera> camera = std::make_shared<GameCamera>();
	m_objList.push_back(camera);

	// 空
	std::shared_ptr<SkySphere> sky = std::make_shared<SkySphere>();
	m_objList.push_back(sky);

	// 地面
	std::shared_ptr<Ground> ground = std::make_shared<Ground>();
	camera->RegistHitObjList(ground);
	m_objList.push_back(ground);

	// チーズ(ゴール)
	std::shared_ptr<Cheese> cheese = std::make_shared<Cheese>();
	m_objList.push_back(cheese);

	// 敵(オオカミ)
	std::shared_ptr<Wolf> wolf = std::make_shared<Wolf>();
	wolf->RegistHitObjList(ground);
	m_objList.push_back(wolf);

	// プレイヤー(ネズミ)
	std::shared_ptr<Mouse> mouse = std::make_shared<Mouse>();
	// プレイヤーを対象に設定するもの
	sky->SetTarget(mouse);
	camera->SetTarget(mouse);
	wolf->RegistHitObjList(mouse);

	// プレイヤーが対象に設定するもの
	mouse->RegistHitObjList(ground);
	mouse->RegistHitObjList(cheese);
	mouse->SetCamera(camera);
	
	m_objList.push_back(mouse);
}
