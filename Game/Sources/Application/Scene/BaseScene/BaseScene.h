#pragma once

class BaseScene
{
public:
	BaseScene() { Init(); }
	virtual ~BaseScene(){}

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();

protected:

	virtual void Event();
	virtual void Init();


};