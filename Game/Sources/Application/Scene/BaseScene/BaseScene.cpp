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

	// エフェクト更新
	EffekseerManager::GetInstance().Update();
}

void BaseScene::PreDraw()
{
	for (auto& obj : m_objList)
	{
		obj->PreDraw();
	}

	// 影を付けるオブジェクトの描画
	ShaderManager::GetInstance().m_modelShader.BeginShadow(2048,2048);
	for (auto& obj : m_objList)
	{
		obj->DrawShadow();
	}
}

void BaseScene::Draw()
{
	// モデル描画
	ShaderManager::GetInstance().m_modelShader.Begin();
	for (auto& obj : m_objList)
	{
		obj->Draw();
	}

	// エフェクト描画
	EffekseerManager::GetInstance().Draw();
}

void BaseScene::DrawSprite()
{
	// 2D画像描画
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
