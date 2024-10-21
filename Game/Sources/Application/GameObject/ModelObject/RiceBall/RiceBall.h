#pragma once

#include "../ModelObject.h"

class RiceBall : public ModelObject
{
public:
	RiceBall() { Init(); }
	~RiceBall()override{}

	void Draw()override;

private:
	void Init()override;

};