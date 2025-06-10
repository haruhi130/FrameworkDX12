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
	/// <summary>
	/// 描画関係の初期化
	/// </summary>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <param name="width">画面横幅</param>
	/// <param name="height">画面縦幅</param>
	/// <returns>初期化に成功したらtrue</returns>
	bool Init(HWND hWnd, int width, int height);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void Prepare();

	/// <summary>
	/// スワップチェーン切替
	/// </summary>
	void ScreenFlip();

	/// <summary>
	/// コマンドキュー待機
	/// </summary>
	void WaitForCommandQueue();

	/// <summary>
	/// デバッグレイヤー有効化
	/// </summary>
	void EnableDebugLayer();

	/// <summary>
	/// スクリーン座標からワールド座標への変換
	/// </summary>
	/// <param name="screenPos">マウス座標</param>
	/// <param name="projZ">奥行き情報</param>
	/// <param name="dst">結果格納</param>
	/// <param name="mCam">ビュー行列</param>
	/// <param name="mProj">プロジェクション行列</param>
	void ClientToWorld(const POINT& screenPos, float projZ, Math::Vector3& dst, const Math::Matrix& mCam, const Math::Matrix& mProj);

	/// <summary>
	/// リソースバリア切替
	/// </summary>
	/// <param name="pResource">切り替えるリソース</param>
	/// <param name="before">切替前のステート</param>
	/// <param name="after">切替後のステート</param>
	void SetResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES before,
		D3D12_RESOURCE_STATES after);

	/// <summary>
	/// バックバッファ色変更
	/// </summary>
	/// <param name="col">変更色</param>
	inline void SetBackBufferColor(const Math::Color& col)
	{ m_clearColor = col; }

	/// <summary>
	/// デバイス取得
	/// </summary>
	/// <returns>デバイス</returns>
	inline ID3D12Device14* GetDevice() const
	{ return m_cpDevice.Get(); }

	/// <summary>
	/// コマンドリスト取得
	/// </summary>
	/// <returns>コマンドリスト</returns>
	inline ID3D12GraphicsCommandList10* GetCmdList() const
	{ return m_cpCmdList.Get(); }

	/// <summary>
	/// コマンドキュー取得
	/// </summary>
	/// <returns>コマンドキュー</returns>
	inline ID3D12CommandQueue* GetCmdQueue() const
	{ return m_cpCmdQueue.Get(); }

	/// <summary>
	/// バックバッファ取得
	/// </summary>
	/// <returns>バックバッファ</returns>
	inline std::vector<ComPtr<ID3D12Resource>> GetBackBuffers()
	{ return m_cpBackBuffers; }

	/// <summary>
	/// RTVヒープ取得
	/// </summary>
	/// <returns>RTVヒープ</returns>
	inline RTVHeap* GetRTVHeap() const
	{ return m_upRTVHeap.get(); }

	/// <summary>
	/// CBVSRVUAVヒープ取得 
	/// </summary>
	/// <returns>CBVSRVUAVヒープ</returns>
	inline CBVSRVUAVHeap* GetCBVSRVUAVHeap() const
	{ return m_upCBVSRVUAVHeap.get(); }
	
	/// <summary>
	/// DSVヒープ取得
	/// </summary>
	/// <returns> DSVヒープ</returns>
	inline DSVHeap* GetDSVHeap() const
	{ return m_upDSVHeap.get(); }

	/// <summary>
	/// ImGuiヒープ取得
	/// </summary>
	/// <returns>ImGuiヒープ</returns>
	inline ImGuiHeap* GetImGuiHeap() const
	{ return m_upImGuiHeap.get(); }

	/// <summary>
	/// 定数バッファアロケーター取得
	/// </summary>
	/// <returns>定数バッファアロケーター</returns>
	inline ConstantBufferAllocator* GetConstantBufferAllocator() const
	{ return m_upCBufferAllocator.get(); }

	/// <summary>
	/// 深度バッファ取得
	/// </summary>
	/// <returns>深度バッファ</returns>
	inline DepthStencil* GetDepthStencil() const
	{ return m_upDepthStencil.get(); }

	/// <summary>
	/// 影用深度バッファ取得
	/// </summary>
	/// <returns>影用深度バッファ</returns>
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

	/// <summary>
	/// ファクトリー作成
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool CreateFactory();

	/// <summary>
	/// デバイス作成
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool CreateDevice();
	
	/// <summary>
	/// コマンドアロケーター作成
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool CreateCommandAllocator();

	/// <summary>
	/// コマンドリスト作成
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool CreateCommandList();

	/// <summary>
	/// コマンドキュー作成
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool CreateCommandQueue();

	/// <summary>
	/// スワップチェーン作成
	/// </summary>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <param name="width">画面横幅</param>
	/// <param name="height">画面縦幅</param>
	/// <returns>成功したらtrue</returns>
	bool CreateSwapChain(HWND hWnd, int width, int height);

	/// <summary>
	/// レンダーターゲットビュー作成
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool CreateRTV();

	/// <summary>
	/// フェンス作成
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool CreateFence();

	//----------------------------------------------------------------
	// デバイス
	ComPtr<IDXGIFactory7> m_cpDxgiFactory = nullptr;
	ComPtr<ID3D12Device14> m_cpDevice = nullptr;

	// DirectX12 Command
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