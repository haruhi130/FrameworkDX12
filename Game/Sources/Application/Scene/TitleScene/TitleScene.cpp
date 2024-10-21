#include "TitleScene.h"
#include "../SceneManager.h"

#include "../../GameObject/SpriteObject/SpriteObject.h"

void TitleScene::Event()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Game);
		ShowCursor(false);
	}
}

void TitleScene::Init()
{
	ShowCursor(true);

	// É^ÉCÉgÉãÉçÉS
	std::shared_ptr<SpriteObject> cursor = std::make_shared<SpriteObject>();
	cursor->SetPos({ 0.0f,80.0f });
	cursor->SetRectangle({ 0,0,960,480 });
	cursor->SetTexture("Assets/Textures/logo.png");
	m_objList.push_back(cursor);

	// Press Enter
	cursor = std::make_shared<SpriteObject>();
	cursor->SetPos({ 0,-200 });
	cursor->SetRectangle({ 0,0,420,78 });
	cursor->SetTexture("Assets/Textures/press.png");
	m_objList.push_back(cursor);

	// îwåi
	cursor = std::make_shared<SpriteObject>();
	cursor->SetPos({ 0,0 });
	cursor->SetRectangle({ 0,0,1280,720 });
	cursor->SetTexture("Assets/Textures/back.png");
	m_objList.push_back(cursor);
}
