#pragma once

struct AnimationData;
struct GLTFModel;

class ModelData
{
public:
	ModelData(){}
	~ModelData(){}

	struct Node
	{
		std::string Name;

		std::shared_ptr<Mesh> spMesh = nullptr;
		Math::Matrix mLocal;
		Math::Matrix mWorld;
		Math::Matrix mBoneInverseWorld;

		int ParentIdx = -1;
		std::vector<int> Children;

		int BoneIdx = -1;
		bool isSkinMesh = false;
	};

	// モデルロード
	bool Load(const std::string& filePath);

	void CreateNodes(const std::shared_ptr<GLTFModel>& spGltfModel);									// ノード作成
	void CreateMaterials(const std::shared_ptr<GLTFModel>& spGltfModel, const  std::string& fileDir);	// マテリアル作成
	void CreateAnimations(const std::shared_ptr<GLTFModel>& spGltfModel);								// アニメーション作成


	// ノード取得
	inline const std::vector<Node>& GetNodes()const 
	{ return m_nodes; }

	inline std::vector<Node>& WorkNodes()
	{ return m_nodes; }

	// アニメーション取得
	const std::shared_ptr<AnimationData> GetAnimation(const std::string& animeName) const;
	const std::shared_ptr<AnimationData> GetAnimation(UINT index) const;

	/*inline std::vector<std::shared_ptr<AnimationData>>& WorkAnimation()
	{ return m_spAnimations; }*/

	inline const std::vector<Material>& GetMaterials() const 
	{ return m_materials; }

	inline const std::vector<int>& GetRootNodeIndices() const 
	{ return m_rootNodeIndices; }
	inline const std::vector<int>& GetBoneNodeIndices() const 
	{ return m_boneNodeIndices; }
	inline const std::vector<int>& GetMeshNodeIndices() const
	{ return m_meshNodeIndices; }

	inline const std::vector<int>& GetCollisionMeshNodeIndices() const 
	{ return m_collisionMeshNodeIndices; }
	
	inline const std::vector<int>& GetDrawMeshNodeIndices() const
	{ return m_drawMeshNodeIndices; }

	bool IsSkinMesh();

private:
	// 全ノード
	std::vector<Node> m_nodes;

	// マテリアル配列
	std::vector<Material> m_materials;

	// アニメーションデータリスト
	std::vector<std::shared_ptr<AnimationData>> m_spAnimations;

	// RootノードのみのIndex配列
	std::vector<int>		m_rootNodeIndices;
	// ボーンノードのみのIndex配列
	std::vector<int>		m_boneNodeIndices;
	// メッシュが存在するノードのみのIndex配列
	std::vector<int>		m_meshNodeIndices;
	// コリジョンメッシュが存在するノードのみのIndex配列
	std::vector<int> m_collisionMeshNodeIndices;
	// 描画するノードのみのIndexn配列
	std::vector<int>		m_drawMeshNodeIndices;

};

class ModelWork
{
public:
	struct Node
	{
		std::string Name;

		Math::Matrix mLocal;
		Math::Matrix mWorld;

		void copy(const ModelData::Node& node)
		{
			Name = node.Name;
			mLocal = node.mLocal;
			mWorld = node.mWorld;
		}
	};

	ModelWork(){}
	ModelWork(const std::shared_ptr<ModelData>& spModelData) { SetModelData(spModelData); }

	void CalcNodeMatrices();
	void RecCalcNodeMatrices(int nodeIdx, int parentNodeIdx = -1);

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

	bool IsNeedCalcNodeMatrices() const { return m_isNeedCalcNode; }

private:
	std::shared_ptr<ModelData> m_spModelData = nullptr;

	std::vector<Node> m_coppiedNodes;

	bool m_isNeedCalcNode = false;
};