#pragma once

#include "../BaseScene/BaseScene.h"

class ResultScene :public BaseScene
{
public:
	ResultScene() { Init(); }
	~ResultScene()override {}

	void ResourceLoad()override;

private:

	void Event()override;
	void Init()override;
};