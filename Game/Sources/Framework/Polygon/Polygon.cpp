#include "Polygon.h"

void Polygon::GetPositions(std::vector<Math::Vector3>& res) const
{
	res.resize(m_vertices.size());

	for (size_t i = 0; i < m_vertices.size(); ++i)
	{
		res[i] = m_vertices[i].Pos;
	}
}
