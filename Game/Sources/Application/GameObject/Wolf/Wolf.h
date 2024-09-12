#pragma once

class Wolf : public BaseObject
{
public:
	Wolf() { Init(); }
	~Wolf()override{}

	void Update()override;
	void PostUpdate()override;

	void Draw()override;

	// Õ“Ë‘ÎÛ‚ğ“o˜^
	void RegistHitObjList(const std::shared_ptr<BaseObject>& obj)
	{
		m_wpHitObjList.push_back(obj);
	}

private:
	void Init()override;
	void UpdateCollision();

	std::shared_ptr<ModelWork> m_spModel = nullptr;
	std::shared_ptr<Animator> m_spAnimator = nullptr;

	// Õ“Ë‘ÎÛƒŠƒXƒg
	std::list<std::weak_ptr<BaseObject>> m_wpHitObjList;
};