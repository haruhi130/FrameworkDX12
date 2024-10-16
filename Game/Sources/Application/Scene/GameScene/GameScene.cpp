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
	std::shared_ptr<Sky> sky = std::make_shared<Sky>();
	m_objList.push_back(sky);

	// �n��
	std::shared_ptr<Stage> stage = std::make_shared<Stage>();
	camera->RegistHitObjList(stage);
	m_objList.push_back(stage);

	// �`�[�Y(�S�[��)
	std::shared_ptr<Cheese> cheese = std::make_shared<Cheese>();
	m_objList.push_back(cheese);

	// ���
	std::shared_ptr<Pedestal> pedestal = std::make_shared<Pedestal>();
	m_objList.push_back(pedestal);

	// �G(�I�I�J�~)
	std::shared_ptr<Wolf> wolf = std::make_shared<Wolf>();
	wolf->RegistHitObjList(stage);
	wolf->RegistHitObjList(pedestal);
	m_objList.push_back(wolf);

	// �v���C���[(�l�Y�~)
	std::shared_ptr<Mouse> mouse = std::make_shared<Mouse>();
	// �v���C���[��Ώۂɐݒ肷�����
	sky->SetTarget(mouse);
	camera->SetTarget(mouse);
	wolf->RegistHitObjList(mouse);

	// �v���C���[���Ώۂɐݒ肷�����
	mouse->RegistHitObjList(stage);
	mouse->RegistHitObjList(cheese);
	mouse->RegistHitObjList(pedestal);
	mouse->SetCamera(camera);
	
	m_objList.push_back(mouse);
}
