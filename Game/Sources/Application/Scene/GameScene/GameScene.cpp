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
	// �V�[���؂�ւ�
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Result);
	}
}

void GameScene::Init()
{
	//ShowCursor(false);

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

	// �G(�I�I�J�~)
	std::shared_ptr<Wolf> wolf = std::make_shared<Wolf>();
	wolf->RegistHitObjList(ground);
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
	mouse->SetCamera(camera);
	
	m_objList.push_back(mouse);
}
