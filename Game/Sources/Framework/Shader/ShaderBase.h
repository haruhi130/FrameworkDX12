#pragma once

#include "Pipeline/Pipeline.h"
#include "RootSignature/RootSignature.h"

struct RenderingSetting
{
	std::vector<InputLayout> InputLayouts;
	std::vector<DXGI_FORMAT> Formats;
	CullMode CullMode = CullMode::Back;
	BlendMode BlendMode = BlendMode::Alpha;
	PrimitiveTopologyType PrimitiveTopologyType = PrimitiveTopologyType::Triangle;
	bool IsDepth = true;
	bool IsDepthMask = true;
	int RTVCount = 1;
	bool IsWireFrame = false;
};

class ShaderBase
{
public:
	virtual bool Init();

	// シェーダー作成
	void Create(std::shared_ptr<RootSignature> spRootSignature, std::shared_ptr<Pipeline> spPipeline,const std::wstring& filePath,
		const RenderingSetting& renderingSetting, const std::vector<RangeType>& rangeTypes);

	// 描画開始
	virtual void Begin(std::shared_ptr<RootSignature> spRootSignature, std::shared_ptr<Pipeline> spPipeline,
		int w = windowWidth, int h = windowHeight);

	// CBVカウント取得
	inline UINT GetCBVCount()const 
	{ return m_cbvCount; }

protected:
	// シェーダーファイル読込
	void LoadShaderFile(const std::wstring& filePath,const std::string& entryPoint = "main");

	std::shared_ptr<Pipeline>		m_spPipeline = nullptr;
	std::shared_ptr<RootSignature>	m_spRootSignature = nullptr;

	ComPtr<ID3DBlob> m_cpVSBlob = nullptr;	// 頂点シェーダー
	ComPtr<ID3DBlob> m_cpHSBlob = nullptr;	// ハルシェーダー
	ComPtr<ID3DBlob> m_cpDSBlob = nullptr;	// ドメインシェーダー
	ComPtr<ID3DBlob> m_cpGSBlob = nullptr;	// ジオメトリシェーダー
	ComPtr<ID3DBlob> m_cpPSBlob = nullptr;	// ピクセルシェーダー

	UINT m_cbvCount = 0;
};