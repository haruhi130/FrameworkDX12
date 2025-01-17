#pragma once

#include "../ModelObject.h"

class Stage : public ModelObject
{
public:
	Stage() { Init(); }
	~Stage()override{}

private:
	void Init()override;


};