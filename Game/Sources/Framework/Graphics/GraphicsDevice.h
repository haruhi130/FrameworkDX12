#pragma once

class RTVHeap;

class GraphicsDevice
{
public:

	// ������
	bool Init(HWND hWnd, int width, int height);

	// �`��O����
	void Prepare();

	// �X���b�v�`�F�[���؂�ւ�
	void ScreenFlip();

	// �R�}���h�L���[�ҋ@
	void WaitForCommandQueue();

	// �f�o�b�O�p
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