#include "TitleScene.h"
#include "../SceneManager.h"

#include "../../GameObject/Test/Test.h"

void TitleScene::Event()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::GetInstance().SetNextScene(SceneManager::SceneType::Game);
	}
}

void TitleScene::Init()
{
	std::shared_ptr<Test> test = std::make_shared<Test>();
	test->SetRectangle({ 0,0,1600,900 });
	test->SetPos({ 0,0 });
	m_objList.push_back(test);
}
