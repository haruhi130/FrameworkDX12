#include "TitleScene.h"
#include "../SceneManager.h"

#include "../../GameObject/SpriteObject/SpriteObject.h"

void TitleScene::Event()
{
	m_bgm->SetVolume(SceneManager::GetInstance().GetBGMVolume());

	///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
	// ÉVÅ[ÉìêÿÇËë÷Ç¶
	if (InputManager::GetInstance().IsPress("LClick"))
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Game);
		ShowCursor(false);
	}
	///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
}

void TitleScene::Init()
{
	ShowCursor(true);

	// îwåi
	std::shared_ptr<SpriteObject> sprite = std::make_shared<SpriteObject>();
	sprite->SetRectangle({ 0,0,1280,720 });
	sprite->SetTexture("Assets/Textures/TitleLogo.png");
	m_objList.push_back(sprite);

	// Press Click
	sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 0,-200 });
	sprite->SetRectangle({ 0,0,420,78 });
	sprite->SetTexture("Assets/Textures/press.png");
	m_objList.push_back(sprite);

	AudioManager::GetInstance().StopAllSound();
	m_bgm = AudioManager::GetInstance().Play("Assets/Sounds/TitleBGM.wav", true);
}
