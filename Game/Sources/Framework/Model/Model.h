#pragma once

class ModelData
{
public:
	struct Node
	{
		std::shared_ptr<Mesh> spMesh = nullptr;
	};

	// モデルロード
	bool Load(const std::string& filePath);

	// ノード取得
	inline const std::vector<Node>& GetNodes()const 
	{ return m_nodes; }

private:
	std::vector<Node> m_nodes;

};