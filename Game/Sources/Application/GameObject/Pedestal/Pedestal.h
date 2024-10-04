#pragma once

class Pedestal :public BaseObject
{
public:
	Pedestal() { Init(); }
	~Pedestal(){}

	void Update()override;
	void Draw()override;

private:
	void Init()override;

	std::shared_ptr<ModelWork> m_spModel = nullptr;
};