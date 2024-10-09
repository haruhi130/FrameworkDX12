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
	void DrawSprite();

	void ImGuiUpdate();

	const std::list<std::shared_ptr<BaseObject>>& GetObjList() 
	{ return m_objList; }

	void AddObject(const std::shared_ptr<BaseObject>& obj) 
	{ m_objList.push_back(obj); }

protected:

	virtual void Event();
	virtual void Init();

	std::list<std::shared_ptr<BaseObject>> m_objList;
};