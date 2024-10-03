#include "TitleScene.h"
#include "../SceneManager.h"

void TitleScene::Event()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		GraphicsDevice::GetInstance().SetBackBufferColor({ 0.5f,0.5f,0.5f,1.0f });
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Game);
	}
}

void TitleScene::Init()
{
	
}
