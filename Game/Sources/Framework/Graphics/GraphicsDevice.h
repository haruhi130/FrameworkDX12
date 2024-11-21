#pragma once

class RTVHeap;
class CBVSRVUAVHeap;
class ConstantBufferAllocator;
class DSVHeap;
class ImGuiHeap;

class DepthStencil;

class GraphicsDevice
{
public:
	// 初期化
	bool Init(HWND hWnd, int width, int height);

	// 描画前準備
	void Prepare();

	// スワップチェーン入れ替え
	void ScreenFlip();

	// GPU処理の完了を確認
	void WaitForCommandQueue();

	// デバッグ用
	void EnableDebugLayer();

	// スクリーン座標から3D座標への変換
	void ClientToWorld(const POINT& screenPos, float projZ, Math::Vector3& dst, const Math::Matrix& mCam, const Math::Matrix& mProj);

	// リソースバリア切り替え
	void SetResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES before,
		D3D12_RESOURCE_STATES after);

	// バックバッファ色変更
	inline void SetBackBufferColor(const Math::Color& col)
	{ m_clearColor = col; }

	// デバイス取得
	inline ID3D12Device14* GetDevice() const
	{ return m_cpDevice.Get(); }

	// コマンドリスト取得
	inline ID3D12GraphicsCommandList10* GetCmdList() const
	{ return m_cpCmdList.Get(); }

	// バックバッファ取得
	inline std::vector<ComPtr<ID3D12Resource>> GetBackBuffers()
	{ return m_cpBackBuffers; }

	// RTVヒープ取得
	inline RTVHeap* GetRTVHeap() const
	{ return m_upRTVHeap.get(); }

	// CBVSRVUAVヒープ取得
	inline CBVSRVUAVHeap* GetCBVSRVUAVHeap() const
	{ return m_upCBVSRVUAVHeap.get(); }

	// DSVヒープ取得
	inline DSVHeap* GetDSVHeap() const
	{ return m_upDSVHeap.get(); }

	// ImGuiヒープ取得
	inline ImGuiHeap* GetImGuiHeap() const
	{ return m_upImGuiHeap.get(); }

	// 定数バッファアロケーター取得
	inline ConstantBufferAllocator* GetConstantBufferAllocator() const
	{ return m_upCBufferAllocator.get(); }

	// 深度バッファ取得
	inline DepthStencil* GetDepthStencil() const
	{ return m_upDepthStencil.get(); }

	// 影用深度バッファ取得
	inline DepthStencil* GetLightDepthStencil() const
	{ return m_upLightDepthStencil.get(); }

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
	bool CreateSwapChain(HWND hWnd, int width, int height);

	// レンダーターゲットビュー作成
	bool CreateRTV();

	// フェンス作成
	bool CreateFence();

	//----------------------------------------------------------------
	// デバイス関連
	ComPtr<IDXGIFactory7> m_cpDxgiFactory = nullptr;
	ComPtr<ID3D12Device14> m_cpDevice = nullptr;

	ComPtr<ID3D12CommandAllocator> m_cpCmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList10> m_cpCmdList = nullptr;
	ComPtr<ID3D12CommandQueue> m_cpCmdQueue = nullptr;

	ComPtr<IDXGISwapChain4> m_cpSwapChain = nullptr;

	std::vector<ComPtr<ID3D12Resource>> m_cpBackBuffers = {};

	ComPtr<ID3D12Fence1> m_cpFence = nullptr;
	UINT64 m_fenceVal = 0;

	//----------------------------------------------------------------
	// DescriptorHeap
	std::unique_ptr<RTVHeap> m_upRTVHeap = nullptr;
	std::unique_ptr<CBVSRVUAVHeap> m_upCBVSRVUAVHeap = nullptr;
	std::unique_ptr<DSVHeap> m_upDSVHeap = nullptr;
	std::unique_ptr<ImGuiHeap> m_upImGuiHeap = nullptr;

	// 定数バッファアロケーター
	std::unique_ptr<ConstantBufferAllocator> m_upCBufferAllocator = nullptr;

	// 深度バッファ
	std::unique_ptr<DepthStencil> m_upDepthStencil = nullptr;
	std::unique_ptr<DepthStencil> m_upLightDepthStencil = nullptr;

	// バックバッファ色
	Math::Color m_clearColor = { 1.0f,1.0f,1.0f,1.0f };

	GraphicsDevice() {}
	~GraphicsDevice() {}
public:
	static GraphicsDevice& GetInstance()
	{
		static GraphicsDevice instance;
		return instance;
	}
};