#include "BaseScene.h"
#include "../../Application.h"

void BaseScene::PreUpdate()
{
	auto it = m_objList.begin();

	while (it != m_objList.end())
	{
		if ((*it)->IsExpired())
		{
			it = m_objList.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto& obj : m_objList)
	{
		obj->PreUpdate();
	}
}

void BaseScene::Update()
{
	for (auto& obj : m_objList)
	{
		obj->Update();
	}

	Event();
}

void BaseScene::PostUpdate()
{
	for (auto& obj : m_objList)
	{
		obj->PostUpdate();
	}
}

void BaseScene::PreDraw()
{
	for (auto& obj : m_objList)
	{
		obj->PreDraw();
	}

	// ‰e‚ð•t‚¯‚éƒIƒuƒWƒFƒNƒg‚Ì•`‰æ
	ShaderManager::GetInstance().m_modelShader.BeginShadow(2048,2048);
	for (auto& obj : m_objList)
	{
		obj->DrawShadow();
	}
}

void BaseScene::Draw()
{
	// ƒ‚ƒfƒ‹•`‰æ
	ShaderManager::GetInstance().m_modelShader.Begin();
	for (auto& obj : m_objList)
	{
		obj->Draw();
	}
}

void BaseScene::DrawSprite()
{
	// 2D‰æ‘œ•`‰æ
	ShaderManager::GetInstance().m_spriteShader.Begin();
	for (auto& obj : m_objList)
	{
		obj->DrawSprite();
	}
}

void BaseScene::ImGuiUpdate()
{
	for (auto& obj : m_objList)
	{
		obj->ImGuiUpdate();
	}
}

void BaseScene::Event()
{
}

void BaseScene::Init()
{
}
