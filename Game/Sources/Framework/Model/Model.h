#pragma once

class ModelData
{
public:
	struct Node
	{
		std::shared_ptr<Mesh> spMesh = nullptr;
	};

	// ���f�����[�h
	bool Load(const std::string& filePath);

	// �m�[�h�擾
	inline const std::vector<Node>& GetNodes()const 
	{ return m_nodes; }

private:
	std::vector<Node> m_nodes;

};