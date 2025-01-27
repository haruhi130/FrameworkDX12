#include "GameScene.h"
#include "../SceneManager.h"

#include "../../GameObject/Camera/GameCamera.h"

#include "../../GameObject/ModelObject/Bamboo/Bamboo.h"
#include "../../GameObject/ModelObject/Bench/Bench.h"
#include "../../GameObject/ModelObject/BookShelf/BookShelf.h"
#include "../../GameObject/ModelObject/Box/Box.h"
#include "../../GameObject/ModelObject/BullTank/BullTank.h"
#include "../../GameObject/ModelObject/Cheese/Cheese.h"
#include "../../GameObject/ModelObject/Dumpling/Dumpling.h"
#include "../../GameObject/ModelObject/CirclePedestal/CirclePedestal.h"
#include "../../GameObject/ModelObject/EggNest/EggNest.h"
#include "../../GameObject/ModelObject/Meat/Meat.h"
#include "../../GameObject/ModelObject/Mouse/Mouse.h"
#include "../../GameObject/ModelObject/Pedestal/Pedestal.h"
#include "../../GameObject/ModelObject/RiceBall/RiceBall.h"
#include "../../GameObject/ModelObject/Sky/Sky.h"
#include "../../GameObject/ModelObject/Stage/Stage.h"
#include "../../GameObject/ModelObject/Trex/Trex.h"

#include "../../GameObject/SpriteObject/SpriteObject.h"

void GameScene::Event()
{
	m_bgm->SetVolume(SceneManager::GetInstance().GetBGMVolume());

	if (SceneManager::GetInstance().GetGoalFlg())
	{
		/*if (!EffekseerManager::GetInstance().IsPlaying(""))
		{
			auto goalPosEffect = EffekseerManager::GetInstance().Play("", { 0,0,-38 });
		}*/
	}
}

void GameScene::Init()
{
	ShowCursor(false);

	ShaderManager::GetInstance().SetDirectionalLight({ -1.0f,-1.0f,-0.5f });
	ShaderManager::GetInstance().SetAmbientLight(0.8f);

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
	mouse->SetPos({ 0,0,-37 });
	mouse->SetCamera(camera);
	m_objList.push_back(mouse);
	sky->SetTarget(mouse);
	camera->SetTarget(mouse);

	// ステージ
	std::shared_ptr<Stage> stage = std::make_shared<Stage>();
	m_objList.push_back(stage);
	camera->RegistHitObjList(stage);
	mouse->RegistHitObjList(stage);

	// スーツ牛
	std::shared_ptr<BullTank> bullTank = std::make_shared<BullTank>();
	bullTank->SetScale(200.0f);
	bullTank->SetPos({ -5,0,33 });
	m_objList.push_back(bullTank);
	bullTank->RegistHitObjList(stage);
	bullTank->RegistHitObjList(mouse);

	camera->RegistHitObjList(bullTank);
	mouse->RegistHitObjList(bullTank);

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

	circlePedestal = std::make_shared <CirclePedestal>();
	circlePedestal->SetPos({ 0,0,-3.0f });
	m_objList.push_back(circlePedestal);
	camera->RegistHitObjList(circlePedestal);
	mouse->RegistHitObjList(circlePedestal);

	//// 箱
	//std::shared_ptr<Box> box = std::make_shared<Box>();
	//box->SetPos({ -9,0,15 });
	//m_objList.push_back(box);
	//camera->RegistHitObjList(box);
	//mouse->RegistHitObjList(box);

	// 肉
	for (int i = 0; i < 2; i++)
	{
		std::shared_ptr<Meat> meat = std::make_shared<Meat>();
		meat->SetPos({ -13.5f,3,-32.0f + (10 * i) });
		m_objList.push_back(meat);
		camera->RegistHitObjList(meat);
		mouse->RegistHitObjList(meat);
	}

	// おにぎり
	std::shared_ptr<RiceBall> rice = std::make_shared<RiceBall>();
	rice->SetPos({ 13.5f,3,0 });
	m_objList.push_back(rice);
	camera->RegistHitObjList(rice);
	mouse->RegistHitObjList(rice);

	// チーズ(ゴール)
	std::shared_ptr<Cheese> cheese = std::make_shared<Cheese>();
	cheese->SetRotationY(135.0f);
	cheese->SetPos({ 0.1f, 3.5f, 37.0f });
	m_objList.push_back(cheese);
	camera->RegistHitObjList(cheese);
	mouse->RegistHitObjList(cheese);

	// 卵
	std::shared_ptr<EggNest> egg = std::make_shared<EggNest>();
	egg->SetScale(5);
	egg->SetPos({0,0.5f,-3});
	m_objList.push_back(egg);
	camera->RegistHitObjList(egg);
	mouse->RegistHitObjList(egg);

	// 団子
	std::shared_ptr<Dumpling> dump = std::make_shared<Dumpling>();
	dump->SetPos({ 13.5f,3,5 });
	m_objList.push_back(dump);
	camera->RegistHitObjList(dump);
	mouse->RegistHitObjList(dump);

	// 竹
	for (int i = 0; i < 2; i++)
	{
		std::shared_ptr<Bamboo> bamboo = std::make_shared<Bamboo>();
		bamboo->SetPos({ 13.5f,0,-10.0f + (5 * i)});
		m_objList.push_back(bamboo);
		camera->RegistHitObjList(bamboo);
		mouse->RegistHitObjList(bamboo);
	}

	// 台座肉用
	std::shared_ptr<Pedestal> pedestal = std::make_shared<Pedestal>();
	pedestal->SetScale({ 1,1,3 });
	pedestal->SetPos({ -13.5f,0,-22.0f });
	m_objList.push_back(pedestal);
	camera->RegistHitObjList(pedestal);
	mouse->RegistHitObjList(pedestal);

	pedestal = std::make_shared<Pedestal>();
	pedestal->SetScale({ 1,1,3 });
	pedestal->SetPos({ -13.5f,0,-32.0f });
	m_objList.push_back(pedestal);
	camera->RegistHitObjList(pedestal);
	mouse->RegistHitObjList(pedestal);

	// 台座おにぎり用
	pedestal = std::make_shared<Pedestal>();
	pedestal->SetPos({ 13.5f,0,0 });
	m_objList.push_back(pedestal);
	camera->RegistHitObjList(pedestal);
	mouse->RegistHitObjList(pedestal);

	// 台座ゴール用
	pedestal = std::make_shared<Pedestal>();
	pedestal->SetPos({ 0,0.7f,37.0f });
	m_objList.push_back(pedestal);
	camera->RegistHitObjList(pedestal);
	mouse->RegistHitObjList(pedestal);

	// 台座団子用
	pedestal = std::make_shared<Pedestal>();
	pedestal->SetPos({ 13.5f,0,5 });
	m_objList.push_back(pedestal);
	camera->RegistHitObjList(pedestal);
	mouse->RegistHitObjList(pedestal);

	// 本棚
	for (int i = 0; i < 4; i++)
	{
		std::shared_ptr<BookShelf> shelf = std::make_shared<BookShelf>();
		shelf->SetScale(3);
		shelf->SetRotationY(-90);
		shelf->SetPos({ -14,0,-9 + (4.5f * i) });
		m_objList.push_back(shelf);
		camera->RegistHitObjList(shelf);
		mouse->RegistHitObjList(shelf);
	}

	// ベンチ
	for (int i = 0; i < 3; i++)
	{
		std::shared_ptr<Bench> bench = std::make_shared<Bench>();
		bench->SetRotationY(90);
		bench->SetPos({ 13.0f,0,-34.5f + (7.0f * i) });
		m_objList.push_back(bench);
		camera->RegistHitObjList(bench);
		mouse->RegistHitObjList(bench);
	}

	//---------------------------------------------
	bullTank = std::make_shared<BullTank>();
	bullTank->SetScale(200.0f);
	bullTank->SetPos({ 5, 0, 33 });
	m_objList.push_back(bullTank);
	bullTank->RegistHitObjList(stage);
	bullTank->RegistHitObjList(mouse);

	camera->RegistHitObjList(bullTank);
	mouse->RegistHitObjList(bullTank);

	//---------------------------------------------
	bullTank = std::make_shared<BullTank>();
	bullTank->SetScale(200.0f);
	bullTank->SetRotationY(-90);
	bullTank->SetMovePosition({ -5,0,-19 }, { 5,0,-19 }, 0.1f);
	m_objList.push_back(bullTank);
	bullTank->RegistHitObjList(stage);
	bullTank->RegistHitObjList(mouse);

	camera->RegistHitObjList(bullTank);
	mouse->RegistHitObjList(bullTank);

	//---------------------------------------------
	bullTank = std::make_shared<BullTank>();
	bullTank->SetScale(200.0f);
	bullTank->SetRotationY(90);
	bullTank->SetMovePosition({ 5,0,6 }, { -5,0,6 }, 0.1f);
	m_objList.push_back(bullTank);
	bullTank->RegistHitObjList(stage);
	bullTank->RegistHitObjList(mouse);

	camera->RegistHitObjList(bullTank);
	mouse->RegistHitObjList(bullTank);

	//---------------------------------------------
	bullTank = std::make_shared<BullTank>();
	bullTank->SetScale(200.0f);
	bullTank->SetRotationY(-90);
	bullTank->SetMovePosition({ -5,0,-11 }, { 5,0,-11 }, 0.1f);
	m_objList.push_back(bullTank);
	bullTank->RegistHitObjList(stage);
	bullTank->RegistHitObjList(mouse);

	camera->RegistHitObjList(bullTank);
	mouse->RegistHitObjList(bullTank);

	// カーソル画像
	std::shared_ptr<SpriteObject> sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 0,0 });
	sprite->SetRectangle({ 0,0,32,32 });
	sprite->SetTexture("Assets/Textures/mark.png");
	m_objList.push_back(sprite);

	// 移動キー
	sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ -512,-288 });
	sprite->SetRectangle({ 0,0,256,144 });
	sprite->SetTexture("Assets/Textures/key.png");
	m_objList.push_back(sprite);

	// 音再生
	m_bgm = AudioManager::GetInstance().Play("Assets/Sounds/GameBGM.wav", true);
}
