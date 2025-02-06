#pragma once

#include "../BaseScene/BaseScene.h"

class TitleScene :public BaseScene
{
public:
	TitleScene() { Init(); }
	~TitleScene()override {}

	void ResourceLoad()override;

private:

	void Event()override;
	void Init()override;
};