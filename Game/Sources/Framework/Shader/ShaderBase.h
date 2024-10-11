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
	bool IsStrip = false;
};

class ShaderBase
{
public:
	virtual bool Init();

	// シェーダー作成
	void Create(const std::wstring& filePath,
		const RenderingSetting& renderingSetting, const std::vector<RangeType>& rangeTypes);

	// 描画開始
	virtual void Begin(int w = windowWidth, int h = windowHeight);

	// CBVカウント取得
	inline UINT GetCBVCount()const 
	{ return m_cbvCount; }

protected:
	// シェーダーファイル読込
	void LoadShaderFile(const std::wstring& filePath);

	std::unique_ptr<Pipeline>		m_upPipeline = nullptr;
	std::unique_ptr<RootSignature>	m_upRootSignature = nullptr;

	ComPtr<ID3DBlob> m_pVSBlob = nullptr;	// 頂点シェーダー
	ComPtr<ID3DBlob> m_pHSBlob = nullptr;	// ハルシェーダー
	ComPtr<ID3DBlob> m_pDSBlob = nullptr;	// ドメインシェーダー
	ComPtr<ID3DBlob> m_pGSBlob = nullptr;	// ジオメトリシェーダー
	ComPtr<ID3DBlob> m_pPSBlob = nullptr;	// ピクセルシェーダー

	UINT m_cbvCount = 0;
};