#pragma once

#include "../ModelObject.h"

class Stage : public ModelObject
{
public:
	Stage() { Init(); }
	~Stage()override{}

	void Draw()override;
	void DrawShadow()override;

private:
	void Init()override;


};