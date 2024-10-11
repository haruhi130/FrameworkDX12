#include "GameScene.h"
#include "../SceneManager.h"

#include "../../GameObject/SkySphere/SkySphere.h"
#include "../../GameObject/Camera/GameCamera.h"
#include "../../GameObject/Mouse/Mouse.h"
#include "../../GameObject/Ground/Ground.h"
#include "../../GameObject/Wolf/Wolf.h"
#include "../../GameObject/Cheese/Cheese.h"
#include "../../GameObject/Pedestal/Pedestal.h"

#include "../../GameObject/Test/Test.h"

void GameScene::Event()
{
	// �V�[���؂�ւ�
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

	// �J����
	std::shared_ptr<GameCamera> camera = std::make_shared<GameCamera>();
	m_objList.push_back(camera);

	// ��
	std::shared_ptr<SkySphere> sky = std::make_shared<SkySphere>();
	m_objList.push_back(sky);

	// �n��
	std::shared_ptr<Ground> ground = std::make_shared<Ground>();
	camera->RegistHitObjList(ground);
	m_objList.push_back(ground);

	// �`�[�Y(�S�[��)
	std::shared_ptr<Cheese> cheese = std::make_shared<Cheese>();
	m_objList.push_back(cheese);

	// ���
	std::shared_ptr<Pedestal> pedestal = std::make_shared<Pedestal>();
	m_objList.push_back(pedestal);

	// �G(�I�I�J�~)
	std::shared_ptr<Wolf> wolf = std::make_shared<Wolf>();
	wolf->RegistHitObjList(ground);
	wolf->RegistHitObjList(pedestal);
	m_objList.push_back(wolf);

	// �v���C���[(�l�Y�~)
	std::shared_ptr<Mouse> mouse = std::make_shared<Mouse>();
	// �v���C���[��Ώۂɐݒ肷�����
	sky->SetTarget(mouse);
	camera->SetTarget(mouse);
	wolf->RegistHitObjList(mouse);

	// �v���C���[���Ώۂɐݒ肷�����
	mouse->RegistHitObjList(ground);
	mouse->RegistHitObjList(cheese);
	mouse->RegistHitObjList(pedestal);
	mouse->SetCamera(camera);
	
	m_objList.push_back(mouse);
}
