#pragma once

struct RenderTargetChange
{
	// レンダーターゲット切り替え用のリソース作成
	bool CreateRenderTarget();

	// レンダーターゲット変更
	bool ChangeRenderTarget() const;
	// レンダーターゲットを戻す
	void UndoRenderTarget() const;

	// レンダーターゲット用テクスチャ作成
	bool CreateRTTexture();

	bool CreateDepthTexture();

	// 描画
	void Draw() const;

	std::shared_ptr<Texture> m_spRTTexture = nullptr;
	int m_rtvNum = 0;

	int m_renderTargetSRVNumber = 0;

	std::shared_ptr<Texture> m_spDepthTexture = nullptr;
	int m_depthSRVNumber = 0;
};