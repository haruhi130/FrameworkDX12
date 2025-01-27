#pragma once

#include "../ModelObject.h"

class Bamboo : public ModelObject
{
public:
	Bamboo() { Init(); }
	~Bamboo()override {}

private:
	void Init()override;

};