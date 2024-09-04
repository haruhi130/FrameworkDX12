#pragma once

struct AnimationData;

class ModelData
{
public:
	ModelData(){}
	~ModelData(){}

	struct Node
	{
		std::shared_ptr<Mesh> spMesh = nullptr;
		Math::Matrix mLocal;
		Math::Matrix mWorld;
	};

	// モデルロード
	bool Load(const std::string& filePath);

	// ノード取得
	inline const std::vector<Node>& GetNodes()const 
	{ return m_nodes; }

	inline std::vector<Node>& WorkNodes()
	{ return m_nodes; }

	// アニメーション取得
	const std::shared_ptr<AnimationData> GetAnimation(const std::string& animeName) const;
	const std::shared_ptr<AnimationData> GetAnimation(UINT index) const;

	inline std::vector<std::shared_ptr<AnimationData>>& WorkAnimation()
	{ return m_spAnimations; }

	inline const std::vector<int>& GetCollisionMeshNodeIndices() const 
	{ return m_collisionMeshNodeIndices; }

private:
	std::vector<Node> m_nodes;
	std::vector<std::shared_ptr<AnimationData>> m_spAnimations;

	// コリジョンメッシュが存在するノードのみのIndex配列
	std::vector<int> m_collisionMeshNodeIndices;

};

class ModelWork
{
public:
	struct Node
	{
		Math::Matrix mLocal;
		Math::Matrix mWorld;

		void copy(const ModelData::Node& node)
		{
			mLocal = node.mLocal;
			mWorld = node.mWorld;
		}
	};

	ModelWork(){}
	ModelWork(const std::shared_ptr<ModelData>& spModelData) { SetModelData(spModelData); }

	void SetModelData(const std::shared_ptr<ModelData>& spModelData);
	void SetModelData(std::string_view fileName);

	inline const std::shared_ptr<ModelData> GetModelData()const 
	{ return m_spModelData; }

	// アニメーションデータ取得
	const std::shared_ptr<AnimationData> GetAnimation(std::string animName) const { return !m_spModelData ? nullptr : m_spModelData->GetAnimation(animName); }
	const std::shared_ptr<AnimationData> GetAnimation(int index) const { return !m_spModelData ? nullptr : m_spModelData->GetAnimation(index); }

	inline const std::vector<ModelData::Node>& GetDataNodes() const
	{ return m_spModelData->GetNodes(); }

	inline const std::vector<Node>& GetNodes() const
	{ return m_coppiedNodes; }
	inline std::vector<Node>& WorkNodes() 
	{ return m_coppiedNodes; }

private:
	std::shared_ptr<ModelData> m_spModelData = nullptr;

	std::vector<Node> m_coppiedNodes;
};