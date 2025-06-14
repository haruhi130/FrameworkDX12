#include "ModelShader.h"

bool ModelShader::Init()
{
	ShaderBase::Init();

	// 影描画用深度テクスチャ作成
	m_spShadowTex = std::make_shared<Texture>();
	m_spShadowTex->CreateLightDepthSRV();

	// シェーダーに渡す情報設定
	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL,InputLayout::COLOR,InputLayout::TANGENT,InputLayout::SKININDEX,InputLayout::SKINWEIGHT };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	// モデル描画シェーダー作成
	Create(m_spRootSignature, m_spPipeline, L"ModelShader/ModelShader", renderingSetting,
		{ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,
		RangeType::SRV,RangeType::SRV,RangeType::SRV ,RangeType::SRV ,RangeType::SRV });

	{
		// 影描画シェーダー作成
		ID3DBlob* pErrorBlob = nullptr;
		auto result = D3DCompileFromFile(L"Sources/Framework/Shader/HLSL/ModelShader/ModelShader_VS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "Shadow",
			"vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, m_cpVSBlob.GetAddressOf(), &pErrorBlob);
		if (FAILED(result))
		{
			assert(0 && "頂点シェーダーのコンパイルに失敗しました");
			return false;
		}

		// 座標変換のみのシェーダーなのでRT数は0,FormatはUNKNOWN
		renderingSetting.RTVCount = 0;
		renderingSetting.Formats = { DXGI_FORMAT_UNKNOWN };
		// シャドウマップ用にパイプラインを設定
		m_spPipelineShadow = std::make_shared<Pipeline>();
		m_spPipelineShadow->SetRenderSettings(m_spRootSignature.get(), renderingSetting.InputLayouts,
			renderingSetting.CullMode, renderingSetting.BlendMode, renderingSetting.PrimitiveTopologyType);
		m_spPipelineShadow->Create({ m_cpVSBlob.Get(),m_cpHSBlob.Get() ,m_cpDSBlob.Get() ,m_cpGSBlob.Get(),nullptr }, renderingSetting.Formats,
			renderingSetting.IsDepth, renderingSetting.IsDepthMask, renderingSetting.RTVCount, renderingSetting.IsWireFrame);
	}

	return true;
}

void ModelShader::Begin(int w, int h)
{
	// DescriptorHeap設定
	GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

	// ルートシグネチャとパイプライン設定
	ShaderBase::Begin(m_spRootSignature, m_spPipeline, w, h);

	// カメラ情報転送
	ShaderManager::GetInstance().WriteCBCamera();

	// ライト情報転送
	ShaderManager::GetInstance().WriteCBLight();

	// 影用DepthTextureをシェーダーにセット
	m_spShadowTex->SetToShader(m_cbvCount + 4);
}

void ModelShader::BeginShadow(int w, int h)
{
	// DescriptorHeap設定
	GraphicsDevice::GetInstance().GetCBVSRVUAVHeap()->SetHeap();

	// シャドウマップ用のハンドル取得
	auto handle = GraphicsDevice::GetInstance().GetDSVHeap()->GetCPUHandle(
		GraphicsDevice::GetInstance().GetLightDepthStencil()->GetDSVNumber());

	// レンダーターゲット変更(PixelShaderを使用しない)
	GraphicsDevice::GetInstance().GetCmdList()->OMSetRenderTargets(0, nullptr, false, &handle);

	// シャドウマップ用の深度バッファクリア
	GraphicsDevice::GetInstance().GetLightDepthStencil()->ClearBuffer();

	// ルートシグネチャとパイプライン設定
	ShaderBase::Begin(m_spRootSignature, m_spPipelineShadow, w, h);

	// ライト情報転送
	ShaderManager::GetInstance().WriteLightParams();
}

void ModelShader::DrawMesh(const Mesh* mesh, const Math::Matrix& mWorld, const std::vector<Material>& materials)
{
	if (mesh == nullptr) { return; }

	// シェーダーへ行列を設定
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(0, mWorld);

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

		// サブセット描画
		mesh->DrawSubset(subi);
	}
}

void ModelShader::DrawModel(const ModelData& modelData, const Math::Matrix& mWorld, bool isUseLight)
{
	// ライトを使用するか
	ShaderManager::GetInstance().SetIsUseLight(isUseLight);

	// オブジェクト情報
	if (m_dirtyCBObj)
	{
		GraphicsDevice::GetInstance().GetConstantBufferAllocator()
			->BindAndAttachData(1, m_cbObject);
	}

	// ノード取得
	auto& nodes = modelData.GetNodes();

	// メッシュ描画
	for (auto& nodeIdx : modelData.GetDrawMeshNodeIndices())
	{
		DrawMesh(nodes[nodeIdx].spMesh.get(), nodes[nodeIdx].mWorld * mWorld, modelData.GetMaterials());
	}

	if (m_dirtyCBObj)
	{
		ResetCBObject();
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

	// スキンメッシュ描画
	if (data->IsSkinMesh())
	{
		m_cbObject.IsSkinMesh = data->IsSkinMesh();
		DrawSkinMesh(modelWork);
		m_dirtyCBObj = true;
	}

	// オブジェクト情報
	if (m_dirtyCBObj)
	{
		GraphicsDevice::GetInstance().GetConstantBufferAllocator()
			->BindAndAttachData(1, m_cbObject);
	}

	// ノード取得
	auto& workNodes = modelWork.GetNodes();
	auto& dataNodes = data->GetNodes();

	// メッシュ描画
	for (auto&& nodeIdx : data->GetDrawMeshNodeIndices())
	{
		DrawMesh(dataNodes[nodeIdx].spMesh.get(), workNodes[nodeIdx].mWorld * mWorld, data->GetMaterials());
	}

	if (m_dirtyCBObj)
	{
		ResetCBObject();
	}
}

void ModelShader::DrawSkinMesh(ModelWork& modelWork)
{
	const std::shared_ptr<ModelData>& data = modelWork.GetModelData();

	auto& workNodes = modelWork.GetNodes();
	auto& dataNodes = data->GetNodes();

	for (auto&& nodeIdx : data->GetBoneNodeIndices())
	{
		if (nodeIdx >= maxBoneBufferSize)
		{
			assert(0 && "転送できるボーン上限数をオーバー");
			return;
		}

		auto& dataNode = dataNodes[nodeIdx];
		auto& workNode = workNodes[nodeIdx];

		m_cbSkinMesh.mBones[dataNode.BoneIdx] = dataNode.mBoneInverseWorld * workNode.mWorld;
	}

	// シェーダーへ計算したボーンを設定
	GraphicsDevice::GetInstance().GetConstantBufferAllocator()
		->BindAndAttachData(2, m_cbSkinMesh);
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
		->BindAndAttachData(3, m_cbMaterial);

	// マテリアル画像を設定
	if (material.spBaseColorTex != nullptr)
	{
		material.spBaseColorTex->SetToShader(m_cbvCount);
	}
	if (material.spEmissiveTex != nullptr)
	{
		material.spEmissiveTex->SetToShader(m_cbvCount + 1);
	}
	if (material.spMetallicRoughnessTex != nullptr)
	{
		material.spMetallicRoughnessTex->SetToShader(m_cbvCount + 2);
	}
	if (material.spNormalTex != nullptr)
	{
		material.spNormalTex->SetToShader(m_cbvCount + 3);
	}
}
