#include "FailedResultScene.h"
#include "../SceneManager.h"

#include "../../GameObject/SpriteObject/SpriteObject.h"
#include "../../GameObject/SpriteObject/Loading/Loading.h"

void FailedResultScene::ResourceLoad()
{
	// îwåi
	std::shared_ptr<SpriteObject> sprite = std::make_shared<SpriteObject>();
	sprite->SetRectangle({ 0,0,1280,720 });
	sprite->SetTexture("Assets/Textures/BackFrame.png");
	m_objList.push_back(sprite);

	sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 0,100 });
	sprite->SetRectangle({ 0,0,696,104 });
	sprite->SetTexture("Assets/Textures/failed.png");
	m_objList.push_back(sprite);

	sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 0,-150 });
	sprite->SetRectangle({ 0,0,442,111 });
	sprite->SetTexture("Assets/Textures/PressClick.png");
	sprite->SetAlphaFluct();
	m_objList.push_back(sprite);

	sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 270,-150 });
	sprite->SetRectangle({ 0,0,128,128 });
	sprite->SetTexture("Assets/Textures/T_Mouse_Left_Key_White.png");
	sprite->SetAlphaFluct();
	m_objList.push_back(sprite);

	AudioManager::GetInstance().StopAllSound();
	m_bgm = AudioManager::GetInstance().Play("Assets/Sounds/Failed.wav", true);

	SceneManager::GetInstance().SetIsLoading(false);
}

void FailedResultScene::Event()
{
	m_bgm->SetVolume(SceneManager::GetInstance().GetBGMVolume());
	
	///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
	// ÉVÅ[ÉìêÿÇËë÷Ç¶
	if (InputManager::GetInstance().IsPress("LClick"))
	{
		EffekseerManager::GetInstance().StopAllEffect();
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Title);
		SceneManager::GetInstance().SetGoalFlg(false);
	}
	///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
}

void FailedResultScene::Init()
{
	ShowCursor(true);

	std::shared_ptr<Loading> load = std::make_shared<Loading>();
	load->SetRectangle({ 0,0,1280,720 });
	load->SetTexture("Assets/Textures/BackFrame.png");
	m_objList.push_back(load);

	load = std::make_shared<Loading>();
	load->SetRectangle({ 0,0,128,128 });
	load->SetTexture("Assets/Textures/reload.png");
	load->SetRotation(true);
	m_objList.push_back(load);

	std::thread thread(&FailedResultScene::ResourceLoad, this);
	thread.detach();
}
