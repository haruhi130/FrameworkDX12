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

	std::shared_ptr<SpriteObject> cursor = std::make_shared<SpriteObject>();
	cursor->SetPos({ 0,0 });
	cursor->SetRectangle({ 0,0,693,78 });
	cursor->SetTexture("Assets/Textures/failed.png");
	m_objList.push_back(cursor);

	// ”wŒi
	cursor = std::make_shared<SpriteObject>();
	cursor->SetPos({ 0,0 });
	cursor->SetRectangle({ 0,0,1280,720 });
	cursor->SetTexture("Assets/Textures/back.png");
	m_objList.push_back(cursor);
}
