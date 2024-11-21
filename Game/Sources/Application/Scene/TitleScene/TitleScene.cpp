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

	// îwåi
	std::shared_ptr<SpriteObject> sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 0,0 });
	sprite->SetRectangle({ 0,0,1280,720 });
	sprite->SetTexture("Assets/Textures/TitleLogo.png");
	m_objList.push_back(sprite);

	// Press Enter
	sprite = std::make_shared<SpriteObject>();
	sprite->SetPos({ 0,-200 });
	sprite->SetRectangle({ 0,0,420,78 });
	sprite->SetTexture("Assets/Textures/press.png");
	m_objList.push_back(sprite);
}
