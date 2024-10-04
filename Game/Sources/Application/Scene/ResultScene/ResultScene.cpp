#include "ResultScene.h"
#include "../SceneManager.h"

void ResultScene::Event()
{
	// ƒV[ƒ“Ø‚è‘Ö‚¦
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Title);
	}
}

void ResultScene::Init()
{
	
}
