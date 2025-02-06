#pragma once

#include "../BaseScene/BaseScene.h"

class FailedResultScene : public BaseScene
{
public:
	FailedResultScene() { Init(); }
	~FailedResultScene()override {}

	void ResourceLoad()override;

private:

	void Event()override;
	void Init()override;
};