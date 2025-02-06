#pragma once

#include "../BaseScene/BaseScene.h"

class GameScene :public BaseScene
{
public:
	GameScene() { Init(); }
	~GameScene()override{}

	void ResourceLoad()override;

private:
	void Event()override;
	void Init()override;

	bool m_isOnce = false;
};