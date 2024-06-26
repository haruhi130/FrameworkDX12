#pragma once

class GraphicsDevice
{
public:
	// 初期化
	bool Init();

private:
	// ファクトリー作成
	bool CreateFactory();

	// デバイス作成
	bool CreateDevice();

	enum class GPUTier
	{
		NVIDIA,
		Amd,
		Intel,
		Arm,
		Qualcomm,
		Kind,
	};

	ComPtr<ID3D12Device8> m_cpDevice = nullptr;
	ComPtr<IDXGIFactory6> m_cpDxgiFactory = nullptr;
	ComPtr<IDXGISwapChain4> m_cpSwapChain = nullptr;

	GraphicsDevice(){}
	~GraphicsDevice(){}
public:
	static GraphicsDevice& GetInstance()
	{
		static GraphicsDevice instance;
		return instance;
	}
};