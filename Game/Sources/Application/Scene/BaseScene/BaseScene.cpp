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

	// �G�t�F�N�g�X�V
	EffekseerManager::GetInstance().Update();
}

void BaseScene::PreDraw()
{
	for (auto& obj : m_objList)
	{
		obj->PreDraw();
	}

	// �e��t����I�u�W�F�N�g�̕`��
	ShaderManager::GetInstance().m_modelShader.BeginShadow(2048,2048);
	for (auto& obj : m_objList)
	{
		obj->DrawShadow();
	}
}

void BaseScene::Draw()
{
	// ���f���`��
	ShaderManager::GetInstance().m_modelShader.Begin();
	for (auto& obj : m_objList)
	{
		obj->Draw();
	}

	// �G�t�F�N�g�`��
	EffekseerManager::GetInstance().Draw();
}

void BaseScene::DrawSprite()
{
	// 2D�摜�`��
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
