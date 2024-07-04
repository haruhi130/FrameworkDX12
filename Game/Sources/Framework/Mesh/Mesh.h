#pragma once

struct Vertex
{
	Vertex(Math::Vector3 position,Math::Vector2 uv):Position(position),UV(uv)
	{}

	Math::Vector3 Position;
	Math::Vector2 UV;
};

class Mesh
{
public:
	// メッシュ作成
	void Create();

	// メッシュ描画
	void DrawInstanced() const;

private:
	ComPtr<ID3D12Resource> m_cpVBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_vbView = {};

	ComPtr<ID3D12Resource> m_cpIBuffer = nullptr;
	D3D12_INDEX_BUFFER_VIEW m_ibView = {};

	std::vector<Vertex> m_vertices;

	std::vector<UINT> m_indices;
};