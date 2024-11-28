#pragma once

class EffekseerManager
{
public:
	bool Init();

	bool Load();

	void Play();

	void Update();

	void Draw();

	void End();

	void SetCamera(const std::shared_ptr<Camera>& camera) { m_wpCamera = camera; }

	void Release();

private:
	EffekseerRenderer::RendererRef m_pEfkRenderer = nullptr;

	Effekseer::ManagerRef m_pEfkManager = nullptr;

	Effekseer::RefPtr<EffekseerRenderer::SingleFrameMemoryPool> m_pEfkMemoryPool = nullptr;
	Effekseer::RefPtr<EffekseerRenderer::CommandList> m_pEfkCmdList = nullptr;

	Effekseer::EffectRef m_pEffect = nullptr;
	Effekseer::Handle m_efkHandle = 0;

	std::weak_ptr<Camera> m_wpCamera;

	EffekseerManager(){}
	~EffekseerManager() {/* Release();*/ }
public:
	static EffekseerManager& GetInstance()
	{
		static EffekseerManager instance;
		return instance;
	}
};