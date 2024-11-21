#pragma once

#include "Pipeline/Pipeline.h"
#include "RootSignature/RootSignature.h"

struct RenderingSetting
{
	std::vector<InputLayout> InputLayouts;			// 頂点レイアウト
	std::vector<DXGI_FORMAT> Formats;				// フォーマット
	CullMode CullMode = CullMode::Back;				// カリング設定(デフォルト:背面カリング)
	BlendMode BlendMode = BlendMode::Alpha;			// ブレンド設定(デフォルト:通常合成)
	PrimitiveTopologyType PrimitiveTopologyType 
				= PrimitiveTopologyType::Triangle;	// トポロジー設定(デフォルト:三角形)
	bool IsDepth = true;							// 深度使用フラグ(デフォルト:使用)
	bool IsDepthMask = true;
	int RTVCount = 1;								// レンダーターゲット数(デフォルト:1)
	bool IsWireFrame = false;
};

class ShaderBase
{
public:
	// 初期化
	virtual bool Init();

	// シェーダー作成
	void Create(const std::shared_ptr<RootSignature>& spRootSignature, const std::shared_ptr<Pipeline>& spPipeline,
		const std::wstring& filePath,const RenderingSetting& renderingSetting, const std::vector<RangeType>& rangeTypes);

	// 描画開始
	virtual void Begin(const std::shared_ptr<RootSignature>& spRootSignature, const std::shared_ptr<Pipeline>& spPipeline,
		int w = window_width, int h = window_height);

	// CBVカウント取得
	inline UINT GetCBVCount() const 
	{ return m_cbvCount; }

protected:
	// シェーダーファイル読込
	void LoadShaderFile(const std::wstring& filePath,const std::string& entryPoint = "main");

	// ルートシグネチャ
	std::shared_ptr<RootSignature>	m_spRootSignature = nullptr;
	// パイプライン
	std::shared_ptr<Pipeline>		m_spPipeline = nullptr;

	ComPtr<ID3DBlob> m_cpVSBlob = nullptr;	// 頂点シェーダー
	ComPtr<ID3DBlob> m_cpHSBlob = nullptr;	// ハルシェーダー
	ComPtr<ID3DBlob> m_cpDSBlob = nullptr;	// ドメインシェーダー
	ComPtr<ID3DBlob> m_cpGSBlob = nullptr;	// ジオメトリシェーダー
	ComPtr<ID3DBlob> m_cpPSBlob = nullptr;	// ピクセルシェーダー

	UINT m_cbvCount = 0;
};