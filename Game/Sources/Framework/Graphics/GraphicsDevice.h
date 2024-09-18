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
	// ������
	bool Init(HWND hWnd, int width, int height);

	// �`��O����
	void Prepare();

	// �X���b�v�`�F�[������ւ�
	void ScreenFlip();

	// GPU�����̊������m�F
	void WaitForCommandQueue();

	// �f�o�b�O�p
	void EnableDebugLayer();

	// �o�b�N�o�b�t�@�F�ύX
	inline void SetBackBufferColor(const Math::Color& col) 
	{ m_clearColor = col; }

	// �f�o�C�X�擾
	inline ID3D12Device14* GetDevice() const
	{ return m_cpDevice.Get(); }

	// �R�}���h���X�g�擾
	inline ID3D12GraphicsCommandList10* GetCmdList()const
	{ return m_cpCmdList.Get(); }

	// CBVSRVUAV�q�[�v�擾
	inline CBVSRVUAVHeap* GetCBVSRVUAVHeap()const 
	{ return m_upCBVSRVUAVHeap.get(); }

	// �萔�o�b�t�@�A���P�[�^�[�擾
	inline ConstantBufferAllocator* GetConstantBufferAllocator()const
	{ return m_upCBufferAllocator.get(); }

	// DSV�q�[�v�擾
	inline DSVHeap* GetDSVHeap()const
	{ return m_upDSVHeap.get(); }

	inline ImGuiHeap* GetImGuiHeap() const
	{ return m_upImGuiHeap.get(); }

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

	// �t�@�N�g���[�쐬
	bool CreateFactory();

	// �f�o�C�X�쐬
	bool CreateDevice();

	// �R�}���h�A���P�[�^�[�쐬
	bool CreateCommandAllocator();

	// �R�}���h���X�g�쐬
	bool CreateCommandList();

	// �R�}���h�L���[�쐬
	bool CreateCommandQueue();

	// �X���b�v�`�F�[���쐬
	bool CreateSwapChain(HWND hWnd,int width,int height);

	// �����_�[�^�[�Q�b�g�r���[�쐬
	bool CreateRTV();

	// �t�F���X�쐬
	bool CreateFence();

	// ���\�[�X�o���A�؂�ւ�
	void SetResourceBarrier(ID3D12Resource* pResource,D3D12_RESOURCE_STATES before,
		D3D12_RESOURCE_STATES after);

	ComPtr<IDXGIFactory7> m_cpDxgiFactory = nullptr;
	ComPtr<ID3D12Device14> m_cpDevice = nullptr;

	ComPtr<ID3D12CommandAllocator> m_cpCmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList10> m_cpCmdList = nullptr;
	ComPtr<ID3D12CommandQueue> m_cpCmdQueue = nullptr;

	ComPtr<IDXGISwapChain4> m_cpSwapChain = nullptr;

	std::vector<ComPtr<ID3D12Resource>> m_pBackBuffers = {};

	std::unique_ptr<RTVHeap> m_upRTVHeap = nullptr;
	std::unique_ptr<CBVSRVUAVHeap> m_upCBVSRVUAVHeap = nullptr;

	std::unique_ptr<ImGuiHeap> m_upImGuiHeap = nullptr;

	ComPtr<ID3D12Fence1> m_cpFence = nullptr;
	UINT64 m_fenceVal = 0;

	std::unique_ptr<ConstantBufferAllocator> m_upCBufferAllocator = nullptr;

	std::unique_ptr<DSVHeap> m_upDSVHeap = nullptr;
	std::unique_ptr<DepthStencil> m_upDepthStencil = nullptr;

	Math::Color m_clearColor = {0.5f,0.5f,0.5f,1.0f};

	GraphicsDevice() {}
	~GraphicsDevice() {}
public:
	static GraphicsDevice& GetInstance()
	{
		static GraphicsDevice instance;
		return instance;
	}
};