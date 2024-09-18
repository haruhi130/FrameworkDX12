#pragma once

class DebugWireFrame
{
public:
	DebugWireFrame(){}
	~DebugWireFrame() { Release(); }

	void AddDebugLine(const Math::Vector3& start, const Math::Vector3& end, const Math::Color& col = { 1,1,1,1 });
	void AddDebugLine(const Math::Vector3& start, const Math::Vector3& dir = Math::Vector3(0,0,1), float length = 1.0f,const Math::Color& col = {1,1,1,1});
	void AddDebugLineFromMatrix(const Math::Matrix& mat, float scale);
	void AddDebugSphere(const Math::Vector3& pos, float radius, const Math::Color& col = { 1,1,1,1 });

	void Draw();

private:
	void Release();

	std::vector<Polygon::Vertex> m_debugVertices;
};