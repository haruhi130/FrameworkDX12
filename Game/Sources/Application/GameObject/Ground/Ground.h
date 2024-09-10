#pragma once

class Ground : public BaseObject
{
public:
	Ground() { Init(); }
	~Ground()override{}

	void Draw()override;

private:
	void Init()override;

	std::shared_ptr<ModelWork> m_spModel = nullptr;
};