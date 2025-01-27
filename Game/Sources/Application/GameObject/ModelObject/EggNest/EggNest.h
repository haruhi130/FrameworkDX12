#pragma once

#include "../ModelObject.h"

class EggNest : public ModelObject
{
public:
	EggNest() { Init(); }
	~EggNest()override {}

private:
	void Init()override;

};