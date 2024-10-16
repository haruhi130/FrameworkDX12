#pragma once

class PostProcessShader : public ShaderBase
{
public:
	bool Init()override;

	void Begin(int w = windowWidth, int h = windowHeight);

	void PreDraw();
	void Draw();
	void PostProcess();

private:
	RenderTargetChange m_change;

};