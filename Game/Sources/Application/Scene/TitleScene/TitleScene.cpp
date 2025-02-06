#include "TitleScene.h"
#include "../SceneManager.h"

#include "../../GameObject/SpriteObject/SpriteObject.h"
#include "../../GameObject/SpriteObject/Loading/Loading.h"

void TitleScene::ResourceLoad()
{
	ShowCursor(true);

	// �w�i
	std::shared_ptr<SpriteObject> sprite = std::make_shared<SpriteObject>();
	sprite->SetRectangle({ 0,0,1280,720 });
	sprite->SetTexture("Assets/Textures/TitleLogo.png");
	m_objList.push_back(sprite);

	// Press Click
	sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 0,-200 });
	sprite->SetRectangle({ 0,0,128,128 });
	sprite->SetTexture("Assets/Textures/T_Mouse_Left_Key_White.png");
	sprite->SetAlphaFluct();
	m_objList.push_back(sprite);

	AudioManager::GetInstance().StopAllSound();
	m_bgm = AudioManager::GetInstance().Play("Assets/Sounds/TitleBGM.wav", true);

	SceneManager::GetInstance().SetIsLoading(false);
}

void TitleScene::Event()
{
	m_bgm->SetVolume(SceneManager::GetInstance().GetBGMVolume());

	///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
	// �V�[���؂�ւ�
	if (InputManager::GetInstance().IsPress("Return"))
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Game);
		ShowCursor(false);
	}
	///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
}

void TitleScene::Init()
{
	std::shared_ptr<Loading> load = std::make_shared<Loading>();
	load->SetPos({ 0,0 });
	load->SetRectangle({ 0,0,1280,720 });
	load->SetTexture("Assets/Textures/Loading.png");
	m_objList.push_back(load);

	std::thread t(&TitleScene::ResourceLoad, this);
	t.detach();
}
