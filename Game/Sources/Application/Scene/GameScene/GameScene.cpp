#include "GameScene.h"
#include "../SceneManager.h"

#include "../../GameObject/Camera/GameCamera.h"

#include "../../GameObject/ModelObject/Mouse/Mouse.h"
#include "../../GameObject/ModelObject/Stage/Stage.h"
#include "../../GameObject/ModelObject/Wolf/Wolf.h"
#include "../../GameObject/ModelObject/Sky/Sky.h"
#include "../../GameObject/ModelObject/Cheese/Cheese.h"
#include "../../GameObject/ModelObject/Pedestal/Pedestal.h"
#include "../../GameObject/ModelObject/Box/Box.h"
#include "../../GameObject/ModelObject/Meat/Meat.h"
#include "../../GameObject/ModelObject/RiceBall/RiceBall.h"

#include "../../GameObject/SpriteObject/SpriteObject.h"

void GameScene::Event()
{
	// �V�[���؂�ւ�
	if (GetAsyncKeyState('O') & 0x8000)
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Clear);
	}
	// �V�[���؂�ւ�
	if (GetAsyncKeyState('L') & 0x8000)
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Failed);
	}
}

void GameScene::Init()
{
	ShowCursor(false);

	ShaderManager::GetInstance().SetAmbientLight(0.7f);

	// �J����
	std::shared_ptr<GameCamera> camera = std::make_shared<GameCamera>();
	m_objList.push_back(camera);

	// ��
	std::shared_ptr<Sky> sky = std::make_shared<Sky>();
	m_objList.push_back(sky);

	// �n��
	std::shared_ptr<Stage> stage = std::make_shared<Stage>();
	camera->RegistHitObjList(stage);
	m_objList.push_back(stage);

	// �v���C���[(�l�Y�~)
	std::shared_ptr<Mouse> mouse = std::make_shared<Mouse>();
	sky->SetTarget(mouse);
	camera->SetTarget(mouse);

	// �G(�I�I�J�~)
	std::shared_ptr<Wolf> wolf = std::make_shared<Wolf>();

	// ��
	std::shared_ptr<Meat> meat = std::make_shared<Meat>();
	meat->SetPos({ 15,3.2f,0 });
	m_objList.push_back(meat);

	// ���ɂ���
	std::shared_ptr<RiceBall> rice = std::make_shared<RiceBall>();
	rice->SetPos({ -7,3.2f,10 });
	m_objList.push_back(rice);

	// ��
	std::shared_ptr<Box> box = std::make_shared<Box>();
	box->SetPos({ -15,0,15 });
	m_objList.push_back(box);

	// �`�[�Y(�S�[��)
	std::shared_ptr<Cheese> cheese = std::make_shared<Cheese>();
	m_objList.push_back(cheese);

	// ���(�S�[���p)
	std::shared_ptr<Pedestal> pedestal = std::make_shared<Pedestal>();
	pedestal->SetPos({ 0,0,25 });
	mouse->RegistHitObjList(pedestal);
	wolf->RegistHitObjList(pedestal);
	m_objList.push_back(pedestal);

	// ������p
	pedestal = std::make_shared<Pedestal>();
	pedestal->SetPos({ 15,0,0 });
	mouse->RegistHitObjList(pedestal);
	wolf->RegistHitObjList(pedestal);
	m_objList.push_back(pedestal);

	// ������ɂ���p
	pedestal = std::make_shared<Pedestal>();
	pedestal->SetPos({ -7,0,10 });
	mouse->RegistHitObjList(pedestal);
	wolf->RegistHitObjList(pedestal);
	m_objList.push_back(pedestal);

	// �I�I�J�~���Ώۂɐݒ肷�����
	wolf->RegistHitObjList(stage);
	wolf->RegistHitObjList(box);
	wolf->RegistHitObjList(meat);
	wolf->RegistHitObjList(rice);
	wolf->RegistHitObjList(mouse);
	m_objList.push_back(wolf);

	// �v���C���[���Ώۂɐݒ肷�����
	mouse->RegistHitObjList(stage);
	mouse->RegistHitObjList(cheese);
	mouse->RegistHitObjList(box);
	mouse->RegistHitObjList(meat);
	mouse->RegistHitObjList(rice);
	mouse->SetCamera(camera);

	m_objList.push_back(mouse);

	// �J�[�\���摜
	std::shared_ptr<SpriteObject> sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 0,0 });
	sprite->SetRectangle({ 0,0,32,32 });
	sprite->SetTexture("Assets/Textures/mark.png");
	m_objList.push_back(sprite);

}
