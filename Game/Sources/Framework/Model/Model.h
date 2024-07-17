#pragma once

struct AnimationData;

class ModelData
{
public:
	struct Node
	{
		std::shared_ptr<Mesh> spMesh = nullptr;
		Math::Matrix mLocal;
	};

	// ���f�����[�h
	bool Load(const std::string& filePath);

	// �m�[�h�擾
	inline const std::vector<Node>& GetNodes()const 
	{ return m_nodes; }

	inline std::vector<Node>& WorkNodes()
	{ return m_nodes; }

	// �A�j���[�V�����擾
	const std::shared_ptr<AnimationData> GetAnimation(const std::string& animeName) const;
	const std::shared_ptr<AnimationData> GetAnimation(UINT index) const;

	inline std::vector<std::shared_ptr<AnimationData>>& WorkAnimation()
	{ return m_spAnimations; }

private:
	std::vector<Node> m_nodes;
	std::vector<std::shared_ptr<AnimationData>> m_spAnimations;

};