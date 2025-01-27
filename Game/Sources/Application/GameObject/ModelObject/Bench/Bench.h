#pragma once

#include "../ModelObject.h"

class Bench : public ModelObject
{
public:
	Bench() { Init(); }
	~Bench()override {}

private:
	void Init()override;

};