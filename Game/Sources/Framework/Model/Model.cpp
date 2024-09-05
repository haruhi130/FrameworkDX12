#include "Model.h"
#include "KdGLTFLoader.h"

bool ModelData::Load(const std::string& filePath)
{
	if (filePath.empty()) { return false; }

	std::shared_ptr<GLTFModel> spGltfModel = LoadGLTFModel(filePath);
	if (spGltfModel == nullptr) { return false; }

	CreateNodes(spGltfModel);

	CreateMaterials(spGltfModel, filePath);

	CreateAnimations(spGltfModel);

	return true;
}

void ModelData::CreateNodes(const std::shared_ptr<GLTFModel>& spGltfModel)
{
	m_nodes.resize(spGltfModel->Nodes.size());

	for (UINT i = 0; i < spGltfModel->Nodes.size(); i++)
	{
		// 入力元ノード
		const GLTFNode& rSrcNode = spGltfModel->Nodes[i];

		// 出力先のノード参照
		Node& rDstNode = m_nodes[i];

		if (rSrcNode.IsMesh)
		{
			// メッシュ作成
			rDstNode.spMesh = std::make_shared<Mesh>();

			if (rDstNode.spMesh)
			{
				rDstNode.spMesh->Create(
					rSrcNode.Mesh.Vertices,
					rSrcNode.Mesh.Faces,
					rSrcNode.Mesh.Subsets,
					rSrcNode.Mesh.IsSkinMesh);
			}

			// メッシュノードリストにインデックス登録
			m_meshNodeIndices.push_back(i);

		}

		// ノード情報セット
		rDstNode.Name = rSrcNode.Name;

		rDstNode.mLocal = rSrcNode.LocalTransform;
		rDstNode.mWorld = rSrcNode.WorldTransform;
		rDstNode.mBoneInverseWorld = rSrcNode.InverseBindMatrix;

		rDstNode.isSkinMesh = rSrcNode.Mesh.IsSkinMesh;

		rDstNode.BoneIdx = rSrcNode.BoneNodeIndex;

		rDstNode.ParentIdx = rSrcNode.Parent;
		rDstNode.Children = rSrcNode.Children;

		// 当たり判定用ノード検索
		if (rDstNode.Name.find("COL") != std::string::npos)
		{
			// 判定用ノードに割り当て
			m_collisionMeshNodeIndices.push_back(i);
		}
		else
		{
			// 描画ノードに割り当て
			m_drawMeshNodeIndices.push_back(i);
		}
	}

	for (UINT nodeIdx = 0; nodeIdx < spGltfModel->Nodes.size(); nodeIdx++)
	{
		// ルートノードのIndexリスト
		if (spGltfModel->Nodes[nodeIdx].Parent == -1) { m_rootNodeIndices.push_back(nodeIdx); }

		// ボーンノードのIndexリスト
		int boneIdx = spGltfModel->Nodes[nodeIdx].BoneNodeIndex;

		if (boneIdx >= 0)
		{
			if (boneIdx >= (int)m_boneNodeIndices.size()) { m_boneNodeIndices.resize(static_cast<std::vector<int, std::allocator<int>>::size_type>(boneIdx) + 1); }

			m_boneNodeIndices[boneIdx] = nodeIdx;
		}
	}

	// 当たり判定用ノードが1つも見つからなければ、m_drawMeshNodeと同じ割り当てを行い
	// 見た目 = 当たり判定となる
	if (!m_collisionMeshNodeIndices.size())
	{
		m_collisionMeshNodeIndices = m_drawMeshNodeIndices;
	}
}

void ModelData::CreateMaterials(const std::shared_ptr<GLTFModel>& spGltfModel, const std::string& fileDir)
{
	m_materials.resize(spGltfModel->Materials.size());
	std::string dirPath = GetDirFromPath(fileDir);

	for (UINT i = 0; i < m_materials.size(); ++i)
	{
		const GLTFMaterial& rSrcMaterial = spGltfModel->Materials[i];
		Material& rDstMaterial = m_materials[i];

		rDstMaterial.Name = rSrcMaterial.Name;

		rDstMaterial.spBaseColorTex = std::make_shared<Texture>();
		if (!rSrcMaterial.BaseColorTexName.empty())
		{
			rDstMaterial.spBaseColorTex->Load(dirPath + rSrcMaterial.BaseColorTexName);
		}

		rDstMaterial.spEmissiveTex = std::make_shared<Texture>();
		if (!rSrcMaterial.EmissiveTexName.empty())
		{
			rDstMaterial.spEmissiveTex->Load(dirPath + rSrcMaterial.EmissiveTexName);
		}

		rDstMaterial.spMetallicRoughnessTex = std::make_shared<Texture>();
		if (!rSrcMaterial.MetallicRoughnessTexName.empty())
		{
			rDstMaterial.spMetallicRoughnessTex->Load(dirPath + rSrcMaterial.MetallicRoughnessTexName);
		}

		rDstMaterial.spNormalTex = std::make_shared<Texture>();
		if (!rSrcMaterial.NormalTexName.empty())
		{
			rDstMaterial.spNormalTex->Load(dirPath + rSrcMaterial.NormalTexName);
		}

		// 基本色
		rDstMaterial.BaseColor = rSrcMaterial.BaseColor;
		
		// 自己発光・エミッシブ
		rDstMaterial.Emissive = rSrcMaterial.Emissive;
		
		// 金属性・粗さ
		rDstMaterial.Metallic = rSrcMaterial.Metallic;
		rDstMaterial.Roughness = rSrcMaterial.Roughness;
	}
}

void ModelData::CreateAnimations(const std::shared_ptr<GLTFModel>& spGltfModel)
{
	// アニメーションデータ
	m_spAnimations.resize(spGltfModel->Animations.size());

	for (UINT i = 0; i < m_spAnimations.size(); ++i)
	{
		const GLTFAnimationData& rSrcAnimation = *spGltfModel->Animations[i];

		m_spAnimations[i] = std::make_shared<AnimationData>();
		AnimationData& rDstAnimation = *(m_spAnimations[i]);

		rDstAnimation.m_name = rSrcAnimation.m_name;
		rDstAnimation.m_maxTime = rSrcAnimation.m_maxLength;
		rDstAnimation.m_channels.resize(rSrcAnimation.m_nodes.size());

		for (UINT j = 0; j < rDstAnimation.m_channels.size(); ++j)
		{
			rDstAnimation.m_channels[j].m_nodeOffset = rSrcAnimation.m_nodes[j]->m_nodeOffset;
			rDstAnimation.m_channels[j].m_translations = rSrcAnimation.m_nodes[j]->m_translations;
			rDstAnimation.m_channels[j].m_rotations = rSrcAnimation.m_nodes[j]->m_rotations;
			rDstAnimation.m_channels[j].m_scales = rSrcAnimation.m_nodes[j]->m_scales;
		}
	}
}

const std::shared_ptr<AnimationData> ModelData::GetAnimation(const std::string& animeName) const
{
	for (auto&& anime : m_spAnimations)
	{
		if (anime->m_name == animeName)
		{
			return anime;
		}
	}

	return nullptr;
}

const std::shared_ptr<AnimationData> ModelData::GetAnimation(UINT index) const
{
	return index >= m_spAnimations.size() ? nullptr : m_spAnimations[index];
}

bool ModelData::IsSkinMesh()
{
	for (auto& node : m_nodes)
	{
		if (node.isSkinMesh) { return true; }
	}

	return false;
}

void ModelWork::CalcNodeMatrices()
{
	if (!m_spModelData) { return; }

	for (auto&& nodeIdx : m_spModelData->GetRootNodeIndices())
	{
		RecCalcNodeMatrices(nodeIdx);
	}
	m_isNeedCalcNode = false;
}

void ModelWork::RecCalcNodeMatrices(int nodeIdx, int parentNodeIdx)
{
	auto& data = m_spModelData->GetNodes()[nodeIdx];
	auto& work = m_coppiedNodes[nodeIdx];

	// 親との行列を合成
	if (parentNodeIdx >= 0)
	{
		auto& parent = m_coppiedNodes[data.ParentIdx];

		work.mWorld = work.mLocal * parent.mWorld;
	}
	else
	{
		work.mWorld = work.mLocal;
	}

	for (auto childNodeIdx : data.Children)
	{
		RecCalcNodeMatrices(childNodeIdx, nodeIdx);
	}
}

void ModelWork::SetModelData(const std::shared_ptr<ModelData>& spModelData)
{
	if (!spModelData) { return; }
	m_spModelData = spModelData;

	auto nodeSize = spModelData->GetNodes().size();

	m_coppiedNodes.resize(nodeSize);
	for (auto i = 0; i < nodeSize; ++i)
	{
		m_coppiedNodes[i].copy(spModelData->GetNodes()[i]);
	}

	m_isNeedCalcNode = true;
}

void ModelWork::SetModelData(std::string_view fileName)
{
	SetModelData(Assets::GetInstance().m_modelDatas.LoadData(fileName));
}
