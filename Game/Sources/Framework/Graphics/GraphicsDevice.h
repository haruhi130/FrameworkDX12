#pragma once

class RTVHeap;

class GraphicsDevice
{
public:

	// 初期化
	bool Init(HWND hWnd, int width, int height);

	// 描画前準備
	void Prepare();

	// スワップチェーン切り替え
	void ScreenFlip();

	// コマンドキュー待機
	void WaitForCommandQueue();

	// デバッグ用
	void EnableDebugLayer();

	inline ID3D12Device14* GetDevice() const
	{ return m_cpDevice.Get(); }

	inline ID3D12GraphicsCommandList10* GetCmdList()const
	{ return m_cpCmdList.Get(); }

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

	// レンダーターゲットビュー作成
	bool CreateRTV();

	// フェンス作成
	bool CreateFence();

	// リソースバリア切り替え
	void SetResourceBarrier(ID3D12Resource* pResource,D3D12_RESOURCE_STATES before,
		D3D12_RESOURCE_STATES after);

	ComPtr<IDXGIFactory7> m_cpDxgiFactory = nullptr;
	ComPtr<ID3D12Device14> m_cpDevice = nullptr;

	ComPtr<ID3D12CommandAllocator> m_cpCmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList10> m_cpCmdList = nullptr;
	ComPtr<ID3D12CommandQueue> m_cpCmdQueue = nullptr;

	ComPtr<IDXGISwapChain4> m_cpSwapChain = nullptr;

	std::unique_ptr<RTVHeap> m_upRTVHeap = nullptr;

	std::vector<ID3D12Resource*> m_cpBackBuffers;

	ComPtr<ID3D12Fence1> m_cpFence = nullptr;
	UINT64 m_fenceVal = 0;

	GraphicsDevice() {}
	~GraphicsDevice() {}
public:
	static GraphicsDevice& GetInstance()
	{
		static GraphicsDevice instance;
		return instance;
	}
};