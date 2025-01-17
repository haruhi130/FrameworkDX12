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

#include "../../GameObject/SpriteObject/SpriteObject.h"

void GameScene::Event()
{
	m_bgm->SetVolume(SceneManager::GetInstance().GetBGMVolume());
}

void GameScene::Init()
{
	ShowCursor(false);

	AudioManager::GetInstance().StopAllSound();

	// �J����
	std::shared_ptr<GameCamera> camera = std::make_shared<GameCamera>();

	// �X�J�C�X�t�B�A
	std::shared_ptr<Sky> sky = std::make_shared<Sky>();
	m_objList.push_back(sky);

	// �X�e�[�W
	std::shared_ptr<Stage> stage = std::make_shared<Stage>();
	m_objList.push_back(stage);

	// �J�t�F�E���b�e
	std::shared_ptr<Mouse> mouse = std::make_shared<Mouse>();
	sky->SetTarget(mouse);
	camera->SetTarget(mouse);

	// �X�[�c��
	std::shared_ptr<BullTank> bullTank = std::make_shared<BullTank>();

	// �������i
	std::shared_ptr<Trex> rex = std::make_shared<Trex>();
	rex->SetScale(1.5f);
	rex->SetRotationY(-20.0f);
	rex->SetPos({ 0,0.5f,-8.0f });
	m_objList.push_back(rex);

	// �~���
	std::shared_ptr<CirclePedestal> circlePedestal = std::make_shared <CirclePedestal>();
	circlePedestal->SetPos({ 0,0,-8.0f });
	mouse->RegistHitObjList(circlePedestal);
	bullTank->RegistHitObjList(circlePedestal);
	camera->RegistHitObjList(circlePedestal);
	m_objList.push_back(circlePedestal);

	// ��
	std::shared_ptr<Box> box = std::make_shared<Box>();
	box->SetPos({ -15,0,15 });
	m_objList.push_back(box);

	// ��
	std::shared_ptr<Meat> meat = std::make_shared<Meat>();
	meat->SetPos({ 13.5f,3.2f,-5.0f });
	m_objList.push_back(meat);

	// ���ɂ���
	std::shared_ptr<RiceBall> rice = std::make_shared<RiceBall>();
	rice->SetPos({ -7,3.2f,10 });
	m_objList.push_back(rice);

	// �`�[�Y(�S�[��)
	std::shared_ptr<Cheese> cheese = std::make_shared<Cheese>();
	cheese->SetRotationY(135.0f);
	cheese->SetPos({ 0.1f, 3.8f, 17.0f });
	m_objList.push_back(cheese);

	// ������p
	std::shared_ptr<Pedestal> pedestal = std::make_shared<Pedestal>();
	pedestal->SetPos({ 13.5f,0,-5.0f });
	mouse->RegistHitObjList(pedestal);
	bullTank->RegistHitObjList(pedestal);
	camera->RegistHitObjList(pedestal);
	m_objList.push_back(pedestal);

	// ������ɂ���p
	pedestal = std::make_shared<Pedestal>();
	pedestal->SetPos({ -7,0,10 });
	mouse->RegistHitObjList(pedestal);
	bullTank->RegistHitObjList(pedestal);
	m_objList.push_back(pedestal);

	// ���(�S�[���p)
	pedestal = std::make_shared<Pedestal>();
	pedestal->SetPos({ 0,0.7f,17.0f });
	mouse->RegistHitObjList(pedestal);
	bullTank->RegistHitObjList(pedestal);
	camera->RegistHitObjList(pedestal);
	m_objList.push_back(pedestal);

	camera->RegistHitObjList(pedestal);
	camera->RegistHitObjList(stage);
	camera->RegistHitObjList(box);
	camera->RegistHitObjList(meat);
	camera->RegistHitObjList(rice);
	camera->RegistHitObjList(rex);
	m_objList.push_back(camera);

	// �I�I�J�~���Ώۂɐݒ肷�����
	bullTank->RegistHitObjList(stage);
	bullTank->RegistHitObjList(box);
	bullTank->RegistHitObjList(meat);
	bullTank->RegistHitObjList(rice);
	bullTank->RegistHitObjList(rex);
	bullTank->RegistHitObjList(mouse);
	m_objList.push_back(bullTank);

	// �v���C���[���Ώۂɐݒ肷�����
	mouse->RegistHitObjList(stage);
	mouse->RegistHitObjList(cheese);
	mouse->RegistHitObjList(box);
	mouse->RegistHitObjList(meat);
	mouse->RegistHitObjList(rice);
	mouse->RegistHitObjList(rex);
	mouse->SetCamera(camera);

	m_objList.push_back(mouse);

	// �J�[�\���摜
	std::shared_ptr<SpriteObject> sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 0,0 });
	sprite->SetRectangle({ 0,0,32,32 });
	sprite->SetTexture("Assets/Textures/mark.png");
	m_objList.push_back(sprite);

	// ���Đ�
	m_bgm = AudioManager::GetInstance().Play("Assets/Sounds/Under_line.wav", true);
}
