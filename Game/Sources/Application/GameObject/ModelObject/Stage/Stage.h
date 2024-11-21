#pragma once

#include "../ModelObject.h"

class Stage : public ModelObject
{
public:
	Stage() { Init(); }
	~Stage()override{}

	void Draw()override;

private:
	void Init()override;


};