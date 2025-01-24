#include "GameScene.h"
#include "../SceneManager.h"

#include "../../GameObject/Camera/GameCamera.h"

#include "../../GameObject/ModelObject/Mouse/Mouse.h"
#include "../../GameObject/ModelObject/Stage/Stage.h"
#include "../../GameObject/ModelObject/BullTank/BullTank.h"
#include "../../GameObject/ModelObject/Sky/Sky.h"
#include "../../GameObject/ModelObject/Cheese/Cheese.h"
#include "../../GameObject/ModelObject/Pedestal/Pedestal.h"
#include "../../GameObject/ModelObject/CirclePedestal/CirclePedestal.h"
#include "../../GameObject/ModelObject/Box/Box.h"
#include "../../GameObject/ModelObject/Meat/Meat.h"
#include "../../GameObject/ModelObject/RiceBall/RiceBall.h"
#include "../../GameObject/ModelObject/Trex/Trex.h"
#include "../../GameObject/ModelObject/BookShelf/BookShelf.h"

#include "../../GameObject/SpriteObject/SpriteObject.h"

void GameScene::Event()
{
	m_bgm->SetVolume(SceneManager::GetInstance().GetBGMVolume());

	/*if (SceneManager::GetInstance().GetGoalFlg())
	{
		if (!EffekseerManager::GetInstance().IsPlaying(""))
		{
			auto goalPosEffect = EffekseerManager::GetInstance().Play("", { 0,0,-38 });
		}
	}*/
}

void GameScene::Init()
{
	ShowCursor(false);

	ShaderManager::GetInstance().SetDirectionalLight({ 0,-1.0f,-0.2f });

	AudioManager::GetInstance().StopAllSound();

	// カメラ
	std::shared_ptr<GameCamera> camera = std::make_shared<GameCamera>();
	m_objList.push_back(camera);

	// スカイスフィア
	std::shared_ptr<Sky> sky = std::make_shared<Sky>();
	sky->SetScale(100.0f);
	m_objList.push_back(sky);

	// カフェ・ラッテ
	std::shared_ptr<Mouse> mouse = std::make_shared<Mouse>();
	mouse->SetScale(0.5f);
	mouse->SetPos({ 0,0,-5 });
	mouse->SetCamera(camera);
	m_objList.push_back(mouse);
	sky->SetTarget(mouse);
	camera->SetTarget(mouse);

	// スーツ牛
	std::shared_ptr<BullTank> bullTank = std::make_shared<BullTank>();
	bullTank->SetScale(200.0f);
	bullTank->SetPos({ -5,0,33 });
	bullTank->SetMovePosition({ -5,0,33 }, { -5,0,25 });
	bullTank->SetSpeed(0.1f);
	m_objList.push_back(bullTank);
	camera->RegistHitObjList(bullTank);
	mouse->RegistHitObjList(bullTank);
	
	// ステージ
	std::shared_ptr<Stage> stage = std::make_shared<Stage>();
	m_objList.push_back(stage);
	camera->RegistHitObjList(stage);
	mouse->RegistHitObjList(stage);

	// 恐竜骨格
	std::shared_ptr<Trex> rex = std::make_shared<Trex>();
	rex->SetScale(1.5f);
	rex->SetRotationY(-20.0f);
	rex->SetPos({ 0,0.5f,-28.0f });
	m_objList.push_back(rex);
	camera->RegistHitObjList(rex);
	mouse->RegistHitObjList(rex);
	
	// 円台座
	std::shared_ptr<CirclePedestal> circlePedestal = std::make_shared <CirclePedestal>();
	circlePedestal->SetPos({ 0,0,-28.0f });
	m_objList.push_back(circlePedestal);
	camera->RegistHitObjList(circlePedestal);
	mouse->RegistHitObjList(circlePedestal);
	
	// 箱
	std::shared_ptr<Box> box = std::make_shared<Box>();
	box->SetPos({ -9,0,15 });
	m_objList.push_back(box);
	camera->RegistHitObjList(box);
	mouse->RegistHitObjList(box);
	
	// 肉
	std::shared_ptr<Meat> meat = std::make_shared<Meat>();
	meat->SetPos({ 13.5f,3.2f,-5.0f });
	m_objList.push_back(meat);
	camera->RegistHitObjList(meat);
	mouse->RegistHitObjList(meat);
	
	//// おにぎり
	//std::shared_ptr<RiceBall> rice = std::make_shared<RiceBall>();
	//rice->SetPos({ -7,3.2f,10 });
	//m_objList.push_back(rice);
	//camera->RegistHitObjList(rice);
	//mouse->RegistHitObjList(rice);
	
	// チーズ(ゴール)
	std::shared_ptr<Cheese> cheese = std::make_shared<Cheese>();
	cheese->SetRotationY(135.0f);
	cheese->SetPos({ 0.1f, 3.8f, 37.0f });
	m_objList.push_back(cheese);
	mouse->RegistHitObjList(cheese);

	// 台座肉用
	std::shared_ptr<Pedestal> pedestal_Meat = std::make_shared<Pedestal>();
	pedestal_Meat->SetPos({ 13.5f,0,-5.0f });
	m_objList.push_back(pedestal_Meat);
	camera->RegistHitObjList(pedestal_Meat);
	mouse->RegistHitObjList(pedestal_Meat);

	// 台座おにぎり用
	std::shared_ptr<Pedestal> pedestal_Rice = std::make_shared<Pedestal>();
	pedestal_Rice->SetScale({ 5,2,1 });
	pedestal_Rice->SetPos({ 0,0,0 });
	m_objList.push_back(pedestal_Rice);
	camera->RegistHitObjList(pedestal_Rice);
	mouse->RegistHitObjList(pedestal_Rice);

	// 台座(ゴール用)
	std::shared_ptr<Pedestal> pedestal_Cheese = std::make_shared<Pedestal>();
	pedestal_Cheese->SetPos({ 0,0.7f,37.0f });
	m_objList.push_back(pedestal_Cheese);
	camera->RegistHitObjList(pedestal_Cheese);
	mouse->RegistHitObjList(pedestal_Cheese);

	// 本棚
	for (int i = 0; i < 4; i++)
	{
		std::shared_ptr<BookShelf> shelf = std::make_shared<BookShelf>();
		shelf->SetScale(3);
		shelf->SetRotationY(-90);
		shelf->SetPos({ -14,0,-9 + (4.5f * i)});
		m_objList.push_back(shelf);
		camera->RegistHitObjList(shelf);
		mouse->RegistHitObjList(shelf);
	}

	///////////////////////////////////////////////
	
	bullTank->RegistHitObjList(pedestal_Meat);
	bullTank->RegistHitObjList(pedestal_Rice);
	bullTank->RegistHitObjList(pedestal_Cheese);
	bullTank->RegistHitObjList(stage);
	bullTank->RegistHitObjList(mouse);

	//---------------------------------------------
	bullTank = std::make_shared<BullTank>();
	bullTank->SetScale(200.0f);
	bullTank->SetPos({ 5, 0, 33 });
	m_objList.push_back(bullTank);
	bullTank->SetTarget(mouse);
	bullTank->RegistHitObjList(pedestal_Meat);
	bullTank->RegistHitObjList(pedestal_Rice);
	bullTank->RegistHitObjList(pedestal_Cheese);
	bullTank->RegistHitObjList(stage);
	bullTank->RegistHitObjList(mouse);

	camera->RegistHitObjList(bullTank);
	mouse->RegistHitObjList(bullTank);

	//---------------------------------------------
	bullTank = std::make_shared<BullTank>();
	bullTank->SetScale(200.0f);
	bullTank->SetPos({ 0, 0, 4 });
	m_objList.push_back(bullTank);
	bullTank->SetTarget(mouse);
	bullTank->RegistHitObjList(pedestal_Meat);
	bullTank->RegistHitObjList(pedestal_Rice);
	bullTank->RegistHitObjList(pedestal_Cheese);
	bullTank->RegistHitObjList(stage);
	bullTank->RegistHitObjList(mouse);

	camera->RegistHitObjList(bullTank);
	mouse->RegistHitObjList(bullTank);
	///////////////////////////////////////////////

	// カーソル画像
	std::shared_ptr<SpriteObject> sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 0,0 });
	sprite->SetRectangle({ 0,0,32,32 });
	sprite->SetTexture("Assets/Textures/mark.png");
	m_objList.push_back(sprite);

	// 移動キー
	sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({-512,-288});
	sprite->SetRectangle({ 0,0,256,144 });
	sprite->SetTexture("Assets/Textures/key.png");
	m_objList.push_back(sprite);

	// 音再生
	m_bgm = AudioManager::GetInstance().Play("Assets/Sounds/GameBGM.wav", true);
}
