#pragma once

class Mesh
{
public:
	void Create();

	void DrawInstanced() const;

private:

	ComPtr<ID3D12Resource> m_cpVBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_vbView;

	std::array<Math::Vector3, 3> m_vertices;
};