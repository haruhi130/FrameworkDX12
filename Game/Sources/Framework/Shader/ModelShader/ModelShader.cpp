#include "ModelShader.h"

bool ModelShader::Init()
{
	// シェーダーに渡す情報設定
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL,InputLayout::COLOR,InputLayout::TANGENT,InputLayout::SKININDEX,InputLayout::SKINWEIGHT };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	Create(L"ModelShader/ModelShader", renderingSetting,
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,
		RangeType::SRV,RangeType::SRV,RangeType::SRV ,RangeType::SRV });

	return true;
}

void ModelShader::Begin(int w, int h)
{
	ShaderBase::Begin(w,h);

	// カメラ情報
	ShaderManager::GetInstance().WriteCBCamera();

	// ライト情報
	ShaderManager::GetInstance().WriteCBLight();
}

void ModelShader::DrawMesh(const Mesh* mesh, const Math::Matrix& mWorld, const std::vector<Material>& materials)
{
	if (mesh == nullptr) { return; }

	// シェーダーへ行列を設定
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(1, mWorld);

	// 頂点バッファとインデックスバッファをセット
	mesh->SetToDevice();

	// サブセット描画
	for (UINT subi = 0; subi < mesh->GetSubsets().size(); subi++)
	{
		if (mesh->GetSubsets()[subi].FaceCount == 0)continue;

		// マテリアル取得
		const Material& material = materials[mesh->GetSubsets()[subi].MaterialNo];

		// マテリアルセット
		SetMaterial(material);

		mesh->DrawSubset(subi);
	}
}

void ModelShader::DrawModel(const ModelData& modelData, const Math::Matrix& mWorld, bool isUseLight)
{
	// ライトを使用するか
	ShaderManager::GetInstance().SetIsUseLight(isUseLight);

	// ノード取得
	auto& nodes = modelData.GetNodes();

	// メッシュ描画
	for (auto& nodeIdx : modelData.GetDrawMeshNodeIndices())
	{
		DrawMesh(nodes[nodeIdx].spMesh.get(), nodes[nodeIdx].mWorld * mWorld, modelData.GetMaterials());
	}
}

void ModelShader::DrawModel(ModelWork& modelWork, const Math::Matrix& mWorld, bool isUseLight)
{
	const std::shared_ptr<ModelData>& data = modelWork.GetModelData();

	if (data == nullptr) { return; }

	// ライトを使用するか
	ShaderManager::GetInstance().SetIsUseLight(isUseLight);

	// 再帰処理
	if (modelWork.IsNeedCalcNodeMatrices())
	{
		modelWork.CalcNodeMatrices();
	}

	// スキンメッシュか判別
	{
		ConstantBufferData::ObjectInfo obj;

		obj.IsSkinMesh = data->IsSkinMesh();
		GraphicsDevice::GetInstance().GetConstantBufferAllocator()
			->BindAndAttachData(2, obj);

		// スキンメッシュ描画
		if (data->IsSkinMesh())
		{
			DrawSkinMesh(modelWork);
		}
	}

	// ノード取得
	auto& workNodes = modelWork.GetNodes();
	auto& dataNodes = data->GetNodes();

	// メッシュ描画
	for (auto&& nodeIdx : data->GetDrawMeshNodeIndices())
	{
		DrawMesh(dataNodes[nodeIdx].spMesh.get(), workNodes[nodeIdx].mWorld * mWorld, data->GetMaterials());
	}
}

void ModelShader::DrawSkinMesh(ModelWork& modelWork)
{
	const std::shared_ptr<ModelData>& data = modelWork.GetModelData();

	auto& workNodes = modelWork.GetNodes();
	auto& dataNodes = data->GetNodes();

	ConstantBufferData::BoneInfo bone;
	for (auto&& nodeIdx : data->GetBoneNodeIndices())
	{
		if (nodeIdx >= maxBoneBufferSize)
		{
			assert(0 && "転送できるボーン上限数をオーバー");
			return;
		}

		auto& dataNode = dataNodes[nodeIdx];
		auto& workNode = workNodes[nodeIdx];

		bone.mBones[dataNode.BoneIdx] = dataNode.mBoneInverseWorld * workNode.mWorld;
	}
	// シェーダーへ計算したボーンを設定
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(3, bone);
}

void ModelShader::SetMaterial(const Material& material)
{
	// マテリアル色を設定
	m_cbMaterial.BaseColor = material.BaseColor;
	m_cbMaterial.Emissive = material.Emissive;
	m_cbMaterial.Metallic = material.Metallic;
	m_cbMaterial.Roughness = material.Roughness;

	// マテリアル色をシェーダーへ転送
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(4, m_cbMaterial);

	// マテリアル画像を設定
	material.spBaseColorTex->Set(m_cbvCount);
	material.spNormalTex->Set(m_cbvCount + 1);
	material.spMetallicRoughnessTex->Set(m_cbvCount + 2);
	material.spEmissiveTex->Set(m_cbvCount + 3);
}
