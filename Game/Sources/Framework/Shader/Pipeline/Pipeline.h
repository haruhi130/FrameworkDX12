#pragma once

enum class CullMode
{
	None = D3D12_CULL_MODE_NONE,
	Front = D3D12_CULL_MODE_FRONT,
	Back = D3D12_CULL_MODE_BACK,
};

enum class BlendMode
{
	Add,
	Alpha,
};

enum class InputLayout
{
	POSITION,
	TEXCOORD,
	NORMAL,
	TANGENT,
	COLOR,
	SKININDEX,
	SKINWEIGHT
};

enum class PrimitiveTopologyType
{
	Undefined = D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED,
	Point = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,
	Line = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
	Triangle = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
	Patch = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH,
};

class RootSignature;

class Pipeline
{
public:

	// 描画関係設定
	void SetRenderSettings(RootSignature* pRootSignature,
		const std::vector<InputLayout>& inputLayouts, CullMode cullMode, BlendMode blendMode,
		PrimitiveTopologyType topologyType);

	// パイプライン作成
	void Create(std::vector<ID3DBlob*> pBlobs, const std::vector<DXGI_FORMAT> formats, bool isDepth, bool isDepthMask,
		int rtvCount, bool isWireFrame);

	// パイプライン取得
	inline ID3D12PipelineState* GetPipeline() const 
	{ return m_cpPipelineState.Get(); }

	// トポロジータイプ取得
	inline PrimitiveTopologyType GetTopologyType() const 
	{ return m_topologyType; }

private:

	void SetInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElements,
		const std::vector<InputLayout>& inputLayouts);

	// ブレンドモード設定
	void SetBlendMode(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode);

	RootSignature* m_pRootSignature = nullptr;

	std::vector<InputLayout>	m_inputLayouts;
	CullMode					m_cullMode = CullMode::None;
	BlendMode					m_blendMode = BlendMode::Alpha;
	PrimitiveTopologyType		m_topologyType = PrimitiveTopologyType::Triangle;

	ComPtr<ID3D12PipelineState> m_cpPipelineState = nullptr;
};