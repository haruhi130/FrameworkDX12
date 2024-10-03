#pragma once

class Cheese : public BaseObject
{
public:
	Cheese() { Init(); }
	~Cheese()override{}

	void Update()override;

	void Draw()override;

private:
	void Init()override;

	std::shared_ptr<ModelWork> m_spModel = nullptr;
};