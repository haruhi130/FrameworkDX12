#pragma once

#include "../BaseScene/BaseScene.h"

class GameScene :public BaseScene
{
public:
	GameScene() { Init(); }
	~GameScene()override{}

private:

	void Event()override;
	void Init()override;
};