#pragma once

class Texture : public Buffer
{
public:
	~Texture()override {}

	/// <summary>
	/// ファイルパスからテクスチャを読み込み
	/// </summary>
	/// <param name="fileName">ファイルパス</param>
	/// <returns>読み込みに成功したらtrue</returns>
	bool Load(std::string_view fileName);

	/// <summary>
	/// マルチパスレンダリングに使用するリソース作成
	/// </summary>
	/// <returns>リソース作成に成功したらtrue</returns>
	bool CreateMultiPassResource();

	/// <summary>
	/// レンダーターゲットに使用する板ポリ作成
	/// </summary>
	/// <returns>板ポリ作成に成功したらtrue</returns>
	bool CreateRenderTarget();
	
	// 深度テクスチャ作成
	
	/// <summary>
	/// 深度バッファからSRV作成
	/// </summary>
	/// <returns>作成に成功したらtrue</returns>
	bool CreateDepthSRV();
	/// <summary>
	/// 光源深度バッファからSRV作成
	/// </summary>
	/// <returns>作成に成功したらtrue</returns>
	bool CreateLightDepthSRV();

	/// <summary>
	/// シェーダーリソースとしてセット
	/// </summary>
	/// <param name="index">使用するインデックス番号</param>
	void SetToShader(int index) const;

	/// <summary>
	/// 板ポリを頂点バッファとインデックスバッファにセット
	/// </summary>
	void SetToDevice() const
	{
		GraphicsDevice::GetInstance().GetCmdList()->IASetVertexBuffers(0, 1, &m_VBV);
		GraphicsDevice::GetInstance().GetCmdList()->IASetIndexBuffer(&m_IBV);
	}

	/// <summary>
	/// SRV番号を取得
	/// </summary>
	/// <returns>SRV番号</returns>
	inline int GetSRVNumber() const
	{ return m_SRVNumber; }

	/// <summary>
	/// テクスチャのリソースデスク情報を取得
	/// </summary>
	/// <returns>リソースデスク情報</returns>
	inline const D3D12_RESOURCE_DESC& GetInfo() const
	{ return m_desc; }

private:
	int m_SRVNumber = 0;

	D3D12_RESOURCE_DESC m_desc = {};

	ComPtr<ID3D12Resource> m_VB = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_VBV = {};

	ComPtr<ID3D12Resource> m_IB = nullptr;
	D3D12_INDEX_BUFFER_VIEW m_IBV = {};
};