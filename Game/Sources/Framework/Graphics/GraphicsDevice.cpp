#include "GraphicsDevice.h"

bool GraphicsDevice::Init(HWND hWnd, int width, int height)
{
	if (!CreateFactory())
	{
		assert(0 && "ファクトリー作成失敗");
		return false;
	}
#ifdef _DEBUG
	EnableDebugLayer();
#endif // _DEBUG


	if (!CreateDevice())
	{
		assert(0 && "デバイス作成失敗");
		return false;
	}

	if (!CreateCommandAllocator())
	{
		assert(0 && "コマンドアロケーター作成失敗");
		return false;
	}

	if (!CreateCommandList())
	{
		assert(0 && "コマンドリスト作成失敗");
		return false;
	}

	if (!CreateCommandQueue())
	{
		assert(0 && "コマンドキュー作成失敗");
		return false;
	}

	if (!CreateSwapChain(hWnd,width,height))
	{
		assert(0 && "スワップチェーン作成失敗");
		return false;
	}

	m_upRTVHeap = std::make_unique<RTVHeap>();
	if (!m_upRTVHeap->Create(HeapType::RTV,300))
	{
		assert(0 && "RTVヒープ作成失敗");
		return false;
	}

	m_upCBVSRVUAVHeap = std::make_unique<CBVSRVUAVHeap>();
	if (!m_upCBVSRVUAVHeap->Create(HeapType::CBVSRVUAV, Math::Vector3(300)))
	{
		assert(0 && "CBVSRVUAVヒープ作成失敗");
		return false;
	}

	m_upCBufferAllocator = std::make_unique<ConstantBufferAllocator>();
	m_upCBufferAllocator->Create(m_upCBVSRVUAVHeap.get());

	m_upDSVHeap = std::make_unique<DSVHeap>();
	if (!m_upDSVHeap->Create(HeapType::DSV, 300))
	{
		assert(0 && "DSVヒープ作成失敗");
		return false;
	}

	m_upDepthStencil = std::make_unique<DepthStencil>();
	if (!m_upDepthStencil->Create(Math::Vector2(float(width), float(height))))
	{
		assert(0 && "DepthStencil作成失敗");
		return false;
	}

	if (!CreateRTV())
	{
		assert(0 && "RTV作成失敗");
		return false;
	}

	if (!CreateFence())
	{
		assert(0 && "フェンス作成失敗");
		return false;
	}

	m_upImGuiHeap = std::make_unique<ImGuiHeap>();
	if (!m_upImGuiHeap->Create(HeapType::CBVSRVUAV, 1))
	{
		return false;
	}

	return true;
}

void GraphicsDevice::Prepare()
{
	auto bbIdx = m_cpSwapChain->GetCurrentBackBufferIndex();

	SetResourceBarrier(m_cpBackBuffers[bbIdx].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	auto rtvH = m_upRTVHeap->GetCPUHandle(bbIdx);

	auto dsvH = m_upDSVHeap->GetCPUHandle(m_upDepthStencil->GetDSVNumber());

	m_cpCmdList->OMSetRenderTargets(1, &rtvH, true, &dsvH);

	m_cpCmdList->ClearRenderTargetView(rtvH, m_clearColor, 0, nullptr);
}

void GraphicsDevice::ScreenFlip()
{
	auto bbIdx = m_cpSwapChain->GetCurrentBackBufferIndex();

	SetResourceBarrier(m_cpBackBuffers[bbIdx].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	m_cpCmdList->Close();
	ID3D12CommandList* cmdLists[] = { m_cpCmdList.Get()};
	m_cpCmdQueue->ExecuteCommandLists(1, cmdLists);

	WaitForCommandQueue();

	m_cpCmdAllocator->Reset();
	m_cpCmdList->Reset(m_cpCmdAllocator.Get(), nullptr);

	m_cpSwapChain->Present(1, 0);
}

void GraphicsDevice::WaitForCommandQueue()
{
	m_cpCmdQueue->Signal(m_cpFence.Get(), ++m_fenceVal);

	if (m_cpFence->GetCompletedValue() != m_fenceVal)
	{
		auto event = CreateEvent(nullptr, false, false, nullptr);
		if (!event)
		{
			assert(0 && "イベントエラー");
		}
		m_cpFence->SetEventOnCompletion(m_fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
}

void GraphicsDevice::EnableDebugLayer()
{
	ComPtr<ID3D12Debug> debugLayer = nullptr;

	// デバッグレイヤー有効化
	D3D12GetDebugInterface(IID_PPV_ARGS(debugLayer.GetAddressOf()));
	debugLayer->EnableDebugLayer();
}

void GraphicsDevice::ClientToWorld(const POINT& screenPos, float projZ, Math::Vector3& dst, const Math::Matrix& mCam, const Math::Matrix& mProj)
{
	projZ = std::clamp(projZ, 0.0f, 1.0f);

	Math::Matrix viewInv = mCam;
	Math::Matrix projInv = mProj.Invert();

	Math::Matrix mVP;
	mVP._11 = windowWidth * 0.5f;
	mVP._22 = -windowHeight * 0.5f;
	mVP._41 = windowWidth * 0.5f;
	mVP._42 = windowHeight * 0.5f;

	Math::Matrix viewportInv = mVP.Invert();

	Math::Matrix convertMat = viewportInv * projInv * viewInv;

	Math::Vector3::Transform(
		{ (float)screenPos.x,(float)screenPos.y,projZ }, convertMat, dst);
}

bool GraphicsDevice::CreateFactory()
{
	UINT dxgiFlags = 0;
	dxgiFlags |= DXGI_CREATE_FACTORY_DEBUG;

	// ファクトリー作成
	auto result = CreateDXGIFactory2(dxgiFlags, IID_PPV_ARGS(m_cpDxgiFactory.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool GraphicsDevice::CreateDevice()
{
	// 使用するアダプター
	ComPtr<IDXGIAdapter> pSelectAdapter = nullptr;
	// アダプター列挙
	std::vector<ComPtr<IDXGIAdapter>> pAdapters;
	std::vector<DXGI_ADAPTER_DESC> descs;

	// 使用中のPCのGPUドライバーを検索し格納
	for (UINT index = 0; 1; ++index)
	{
		pAdapters.push_back(nullptr);
		HRESULT hr = m_cpDxgiFactory->EnumAdapters(index, &pAdapters[index]);

		if (hr == DXGI_ERROR_NOT_FOUND) { break; }

		descs.push_back({});
		pAdapters[index]->GetDesc(&descs[index]);
	}

	GPUTier gpuTier = GPUTier::Kind;

	// 優先度の高いGPUドライバーを使用
	for (int i = 0; i < descs.size(); ++i)
	{
		if (std::wstring(descs[i].Description).find(L"NVIDIA") != std::wstring::npos)
		{
			pSelectAdapter = pAdapters[i];
			break;
		}
		else if (std::wstring(descs[i].Description).find(L"Amd") != std::wstring::npos)
		{
			if (gpuTier > GPUTier::Amd)
			{
				pSelectAdapter = pAdapters[i];
				gpuTier = GPUTier::Amd;
			}
		}
		else if (std::wstring(descs[i].Description).find(L"Intel") != std::wstring::npos)
		{
			if (gpuTier > GPUTier::Intel)
			{
				pSelectAdapter = pAdapters[i];
				gpuTier = GPUTier::Intel;
			}
		}
		else if (std::wstring(descs[i].Description).find(L"Arm") != std::wstring::npos)
		{
			if (gpuTier > GPUTier::Arm)
			{
				pSelectAdapter = pAdapters[i];
				gpuTier = GPUTier::Arm;
			}
		}
		else if (std::wstring(descs[i].Description).find(L"Qualcomm") != std::wstring::npos)
		{
			if (gpuTier > GPUTier::Qualcomm)
			{
				pSelectAdapter = pAdapters[i];
				gpuTier = GPUTier::Qualcomm;
			}
		}
	}

	// フィーチャレベル列挙
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// Direct3Dデバイス初期化
	for (auto level : featureLevels)
	{
		if (D3D12CreateDevice(pSelectAdapter.Get(), level, IID_PPV_ARGS(m_cpDevice.ReleaseAndGetAddressOf())) == S_OK)
		{
			break;
		}
	}

	return true;
}

bool GraphicsDevice::CreateCommandAllocator()
{
	// コマンドアロケーター作成
	auto result = m_cpDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_cpCmdAllocator.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool GraphicsDevice::CreateCommandList()
{
	// コマンドリスト作成
	auto result = m_cpDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cpCmdAllocator.Get(), nullptr, IID_PPV_ARGS(m_cpCmdList.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool GraphicsDevice::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; // タイムアウト無し
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL; // プライオリティ指定無し
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // コマンドリストと合わせる

	// コマンドキュー作成
	auto result = m_cpDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(m_cpCmdQueue.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool GraphicsDevice::CreateSwapChain(HWND hWnd, int width, int height)
{
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	ComPtr<IDXGISwapChain1> pSwapChain = nullptr;
	
	// スワップチェーン作成
	auto result = m_cpDxgiFactory->CreateSwapChainForHwnd(m_cpCmdQueue.Get(), hWnd, &swapChainDesc,
		nullptr, nullptr, pSwapChain.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// 型変換チェック
	result = pSwapChain->QueryInterface(IID_PPV_ARGS(m_cpSwapChain.GetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}

	// 使用するバッファ数でresize
	m_cpBackBuffers.resize(swapChainDesc.BufferCount);

	return true;
}

bool GraphicsDevice::CreateRTV()
{
	for (int idx = 0; idx < m_cpBackBuffers.size(); ++idx)
	{
		auto result = m_cpSwapChain->GetBuffer(idx, IID_PPV_ARGS(m_cpBackBuffers[idx].GetAddressOf()));
		if (FAILED(result))
		{
			return false;
		}

		// レンダーターゲットビュー作成
		m_upRTVHeap->CreateRTV(m_cpBackBuffers[idx].Get());
	}

	return true;
}

bool GraphicsDevice::CreateFence()
{
	// フェンス作成
	auto result = m_cpDevice->CreateFence(m_fenceVal,
		D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_cpFence.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void GraphicsDevice::SetResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES before,
	D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Transition.pResource = pResource;
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	m_cpCmdList->ResourceBarrier(1, &barrier);
}
