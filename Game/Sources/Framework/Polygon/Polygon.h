#pragma once

class Polygon
{
public:
	struct Vertex
	{
		Math::Vector3 Pos;
		Math::Vector2 UV;
		unsigned int Color = 0xFFFFFFFF;
		Math::Vector3 Normal = Math::Vector3::Backward;
		Math::Vector3 Tangent = Math::Vector3::Left;
	};

	Polygon(){}
	virtual ~Polygon(){}

	inline const std::vector<Vertex>& GetVertices() const 
	{ return m_vertices; }

	void GetPositions(std::vector<Math::Vector3>& res) const;

	bool IsEnable()const { return (m_isEnable); }
	void SetEnable(bool enable) { m_isEnable = enable; }

	bool Is2DObject()const { return m_2DObject; }
	void Set2DObject(bool is2D) { m_2DObject = is2D; }

protected:

	std::vector<Vertex> m_vertices;

	bool m_isEnable = true;

	bool m_2DObject = true;
};