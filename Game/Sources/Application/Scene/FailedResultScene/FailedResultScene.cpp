#include "FailedResultScene.h"
#include "../SceneManager.h"

#include "../../GameObject/SpriteObject/SpriteObject.h"

void FailedResultScene::Event()
{
	// ƒV[ƒ“Ø‚è‘Ö‚¦
	if (GetAsyncKeyState('P') & 0x8000)
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Title);
	}
}

void FailedResultScene::Init()
{
	ShowCursor(true);

	std::shared_ptr<SpriteObject> sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 0,0 });
	sprite->SetRectangle({ 0,0,693,78 });
	sprite->SetTexture("Assets/Textures/failed.png");
	m_objList.push_back(sprite);

	// ”wŒi
	sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 0,0 });
	sprite->SetRectangle({ 0,0,1280,720 });
	sprite->SetTexture("Assets/Textures/BackFrame.png");
	m_objList.push_back(sprite);
}
