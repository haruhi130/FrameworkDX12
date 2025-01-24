#pragma once

class ModelObject : public BaseObject
{
public:

	ModelObject() { Init(); }
	virtual ~ModelObject()override{}

	virtual void Update()override{}
	virtual void PostUpdate()override{}
	virtual void Draw()override;
	virtual void DrawShadow()override;

	virtual void ImGuiUpdate()override{}

	virtual const std::shared_ptr<ModelWork>& GetModel() const
	{ return m_spModel; }

	void SetScale(float scale)override
	{
		m_scale = { scale,scale,scale };
		m_mWorld *= Math::Matrix::CreateScale(m_scale);
	}

	void SetScale(const Math::Vector3& scale)override
	{
		m_scale = scale;
		m_mWorld *= Math::Matrix::CreateScale(m_scale);
	}

protected:
	virtual void Init()override{}

	virtual void UpdateMatrix(){}
	virtual void UpdateRotate(Math::Vector3& moveVec){}
	virtual void UpdateCollision(){}

	std::shared_ptr<ModelWork> m_spModel = nullptr;
	std::shared_ptr<Animator> m_spAnimator = nullptr;

	Math::Vector3 m_scale = {1.0f,1.0f,1.0f};
};