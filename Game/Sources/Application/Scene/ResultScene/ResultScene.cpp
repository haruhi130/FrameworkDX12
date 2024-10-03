#include "ResultScene.h"
#include "../SceneManager.h"

void ResultScene::Event()
{
	// ƒV[ƒ“Ø‚è‘Ö‚¦
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		GraphicsDevice::GetInstance().SetBackBufferColor({ 0.8f,0.5f,0.3f,1.0f });
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Title);
	}
}

void ResultScene::Init()
{
	
}
