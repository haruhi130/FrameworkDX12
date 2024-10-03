#pragma once

class Dumpling : public BaseObject
{
public:
	Dumpling() { Init(); }
	~Dumpling()override{}

	void Update()override;

	void Draw()override;

private:
	void Init()override;

	std::shared_ptr<ModelWork> m_spModel = nullptr;
};