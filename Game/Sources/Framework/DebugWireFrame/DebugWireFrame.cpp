#include "DebugWireFrame.h"

void DebugWireFrame::AddDebugLine(const Math::Vector3& start, const Math::Vector3& end, const Math::Color& col)
{
	Polygon::Vertex v1;
	v1.Color = col.RGBA().v;
	v1.UV = Math::Vector2::Zero;
	v1.Pos = start;

	Polygon::Vertex v2;
	v2.Color = col.RGBA().v;
	v2.UV = Math::Vector2::Zero;
	v2.Pos = end;

	m_debugVertices.push_back(v1);
	m_debugVertices.push_back(v2);
}

void DebugWireFrame::AddDebugLine(const Math::Vector3& start, const Math::Vector3& dir, float length, const Math::Color& col)
{
	Polygon::Vertex v1;
	v1.Color = col.RGBA().v;
	v1.UV = Math::Vector2::Zero;
	v1.Pos = start;

	Polygon::Vertex v2;
	v2.Color = col.RGBA().v;
	v2.UV = Math::Vector2::Zero;
	v2.Pos = v1.Pos + (dir * length);

	m_debugVertices.push_back(v1);
	m_debugVertices.push_back(v2);
}

void DebugWireFrame::AddDebugLineFromMatrix(const Math::Matrix& mat, float scale)
{
	Math::Vector3 start = mat.Translation();
	Math::Vector3 vAxis;

	// XŽ²•`‰æ
	vAxis = mat.Right();
	vAxis.Normalize();
	AddDebugLine(start, start + vAxis * scale, {1,0,0,1});

	// YŽ²•`‰æ
	vAxis = mat.Up();
	vAxis.Normalize();
	AddDebugLine(start, start + vAxis * scale, { 0,1,0,1 });

	// ZŽ²•`‰æ
	vAxis = mat.Backward();
	vAxis.Normalize();
	AddDebugLine(start, start + vAxis * scale, { 0,0,1,1 });

}

void DebugWireFrame::AddDebugSphere(const Math::Vector3& pos, float radius, const Math::Color& col)
{
	Polygon::Vertex v;
	v.UV = Math::Vector2::Zero;
	v.Color = col.RGBA().v;

	int detail = 16;
	for (int i = 0; i < detail + 1; ++i)
	{
		// XZ–Ê
		v.Pos = pos;
		v.Pos.x += cos(DirectX::XMConvertToRadians((float)i * (360 / detail)) * radius);
		v.Pos.z += sin(DirectX::XMConvertToRadians((float)i * (360 / detail)) * radius);
		m_debugVertices.push_back(v);

		v.Pos = pos;
		v.Pos.x += cos(DirectX::XMConvertToRadians((float)(i+1) * (360 / detail)) * radius);
		v.Pos.z += sin(DirectX::XMConvertToRadians((float)(i + 1) * (360 / detail)) * radius);
		m_debugVertices.push_back(v);

		// XY–Ê
		v.Pos = pos;
		v.Pos.x += cos(DirectX::XMConvertToRadians((float)i * (360 / detail)) * radius);
		v.Pos.y += sin(DirectX::XMConvertToRadians((float)i * (360 / detail)) * radius);
		m_debugVertices.push_back(v);

		v.Pos = pos;
		v.Pos.x += cos(DirectX::XMConvertToRadians((float)(i + 1) * (360 / detail)) * radius);
		v.Pos.y += sin(DirectX::XMConvertToRadians((float)(i + 1) * (360 / detail)) * radius);
		m_debugVertices.push_back(v);

		// YZ–Ê
		v.Pos = pos;
		v.Pos.y +=cos(DirectX::XMConvertToRadians((float)i * (360 / detail)) * radius);
		v.Pos.z +=sin(DirectX::XMConvertToRadians((float)i * (360 / detail)) * radius);
		m_debugVertices.push_back(v);

		v.Pos = pos;
		v.Pos.y += cos(DirectX::XMConvertToRadians((float)(i + 1) * (360 / detail)) * radius);
		v.Pos.z += sin(DirectX::XMConvertToRadians((float)(i + 1) * (360 / detail)) * radius);
		m_debugVertices.push_back(v);
	}
}

void DebugWireFrame::Draw()
{
	if (m_debugVertices.size() < 2) { return; }



	m_debugVertices.clear();
}

void DebugWireFrame::Release()
{
	m_debugVertices.clear();
}
