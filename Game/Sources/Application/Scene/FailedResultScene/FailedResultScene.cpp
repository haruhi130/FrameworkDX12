#include "FailedResultScene.h"
#include "../SceneManager.h"

#include "../../GameObject/SpriteObject/SpriteObject.h"

void FailedResultScene::Event()
{
	m_bgm->SetVolume(SceneManager::GetInstance().GetBGMVolume());
	
	///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
	// ÉVÅ[ÉìêÿÇËë÷Ç¶
	if (InputManager::GetInstance().IsPress("LClick"))
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Title);
		SceneManager::GetInstance().SetGoalFlg(false);
	}
	///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
}

void FailedResultScene::Init()
{
	ShowCursor(true);

	// îwåi
	std::shared_ptr<SpriteObject> sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 0,0 });
	sprite->SetRectangle({ 0,0,1280,720 });
	sprite->SetTexture("Assets/Textures/BackFrame.png");
	m_objList.push_back(sprite);

	sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 0,0 });
	sprite->SetRectangle({ 0,0,693,78 });
	sprite->SetTexture("Assets/Textures/failed.png");
	m_objList.push_back(sprite);

	sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 0,-200 });
	sprite->SetRectangle({ 0,0,128,128 });
	sprite->SetTexture("Assets/Textures/T_Mouse_Left_Key_White.png");
	m_objList.push_back(sprite);

	AudioManager::GetInstance().StopAllSound();
	m_bgm = AudioManager::GetInstance().Play("Assets/Sounds/Failed.wav", true);
}
