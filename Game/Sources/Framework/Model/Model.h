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

	// ���f�����[�h
	bool Load(const std::string& filePath);

	void CreateNodes(const std::shared_ptr<GLTFModel>& spGltfModel);									// �m�[�h�쐬
	void CreateMaterials(const std::shared_ptr<GLTFModel>& spGltfModel, const  std::string& fileDir);	// �}�e���A���쐬
	void CreateAnimations(const std::shared_ptr<GLTFModel>& spGltfModel);								// �A�j���[�V�����쐬

	// �m�[�h�擾
	inline const std::vector<Node>& GetNodes() const 
	{ return m_nodes; }

	inline std::vector<Node>& WorkNodes()
	{ return m_nodes; }

	// �A�j���[�V�����擾
	const std::shared_ptr<AnimationData> GetAnimation(const std::string& animeName) const;
	const std::shared_ptr<AnimationData> GetAnimation(UINT index) const;

	// �}�e���A���z��擾
	inline const std::vector<Material>& GetMaterials() const 
	{ return m_materials; }

	// ���[�g�m�[�h�z��擾
	inline const std::vector<int>& GetRootNodeIndices() const 
	{ return m_rootNodeIndices; }
	
	// �{�[���m�[�h�z��擾
	inline const std::vector<int>& GetBoneNodeIndices() const 
	{ return m_boneNodeIndices; }
	
	// ���b�V���m�[�h�z��擾
	inline const std::vector<int>& GetMeshNodeIndices() const
	{ return m_meshNodeIndices; }

	// �Փ˔���p���b�V���m�[�h�z��擾
	inline const std::vector<int>& GetCollisionMeshNodeIndices() const 
	{ return m_collisionMeshNodeIndices; }
	
	// �`��p���b�V���m�[�h�z��擾
	inline const std::vector<int>& GetDrawMeshNodeIndices() const
	{ return m_drawMeshNodeIndices; }

	// �X�L�����b�V������
	bool IsSkinMesh();

private:
	// �S�m�[�h
	std::vector<Node> m_nodes;

	// �}�e���A���z��
	std::vector<Material> m_materials;

	// �A�j���[�V�����f�[�^���X�g
	std::vector<std::shared_ptr<AnimationData>> m_spAnimations;

	// Root�m�[�h�݂̂�Index�z��
	std::vector<int>		m_rootNodeIndices;
	// �{�[���m�[�h�݂̂�Index�z��
	std::vector<int>		m_boneNodeIndices;
	// ���b�V�������݂���m�[�h�݂̂�Index�z��
	std::vector<int>		m_meshNodeIndices;
	// �R���W�������b�V�������݂���m�[�h�݂̂�Index�z��
	std::vector<int> m_collisionMeshNodeIndices;
	// �`�悷��m�[�h�݂̂�Index�z��
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
	ModelWork(const std::shared_ptr<ModelData>& spModelData) 
	{ SetModelData(spModelData); }

	~ModelWork(){}

	void CalcNodeMatrices();
	void RecCalcNodeMatrices(int nodeIdx, int parentNodeIdx = -1);

	// ���f���ݒ�
	void SetModelData(const std::shared_ptr<ModelData>& spModelData);
	void SetModelData(std::string_view fileName);

	// ���f���擾
	inline const std::shared_ptr<ModelData> GetModelData() const 
	{ return m_spModelData; }

	// �A�j���[�V�����f�[�^�擾
	inline const std::shared_ptr<AnimationData> GetAnimation(std::string animName) const 
	{ return !m_spModelData ? nullptr : m_spModelData->GetAnimation(animName); }
	inline const std::shared_ptr<AnimationData> GetAnimation(int index) const 
	{ return !m_spModelData ? nullptr : m_spModelData->GetAnimation(index); }

	// ���f���f�[�^�̃m�[�h�z��擾
	inline const std::vector<ModelData::Node>& GetDataNodes() const
	{ return m_spModelData->GetNodes(); }

	// �R�s�[�����m�[�h�z��擾
	inline const std::vector<Node>& GetNodes() const
	{ return m_coppiedNodes; }
	inline std::vector<Node>& WorkNodes() 
	{ return m_coppiedNodes; }

	inline bool IsNeedCalcNodeMatrices() const 
	{ return m_isNeedCalcNode; }

private:
	// ���f���f�[�^
	std::shared_ptr<ModelData> m_spModelData = nullptr;

	// �������ω�����\���̂���m�[�h�f�[�^�̃R�s�[�z��
	std::vector<Node> m_coppiedNodes;

	bool m_isNeedCalcNode = false;
};