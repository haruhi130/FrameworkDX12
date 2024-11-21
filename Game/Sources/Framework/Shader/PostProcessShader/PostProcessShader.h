#pragma once

class PostProcessShader : public ShaderBase
{
public:
	// 初期化
	bool Init()override;

	// シェーダー設定
	void Begin(int w = window_width, int h = window_height);

	// レンダーターゲット変更
	void PreDraw();

	// 描画
	void Draw();

	// レンダーターゲット変更
	void PostProcess();

private:
	// レンダーターゲット変更用
	RenderTargetChange m_RTChange;
};