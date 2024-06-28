#pragma once

class GraphicsDevice
{
public:
	// 初期化
	bool Init(HWND hWnd, int width, int height);

private:
	enum class GPUTier
	{
		NVIDIA,
		Amd,
		Intel,
		Arm,
		Qualcomm,
		Kind,
	};

	// ファクトリー作成
	bool CreateFactory();

	// デバイス作成
	bool CreateDevice();

	// コマンドアロケーター作成
	bool CreateCommandAllocator();

	// コマンドリスト作成
	bool CreateCommandList();

	// コマンドキュー作成
	bool CreateCommandQueue();

	// スワップチェーン作成
	bool CreateSwapChain(HWND hWnd,int width,int height);

	ComPtr<IDXGIFactory7> m_cpDxgiFactory = nullptr;
	ComPtr<ID3D12Device10> m_cpDevice = nullptr;

	ComPtr<ID3D12CommandAllocator> m_cpCmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList7> m_cpCmdList = nullptr;
	ComPtr<ID3D12CommandQueue> m_cpCmdQueue = nullptr;

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