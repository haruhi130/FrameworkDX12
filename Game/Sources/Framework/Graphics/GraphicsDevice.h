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

	// �X�N���[�����W����3D���W�ւ̕ϊ�
	void ClientToWorld(const POINT& screenPos, float projZ, Math::Vector3& dst, const Math::Matrix& mCam, const Math::Matrix& mProj);

	// ���\�[�X�o���A�؂�ւ�
	void SetResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES before,
		D3D12_RESOURCE_STATES after);

	// �o�b�N�o�b�t�@�F�ύX
	inline void SetBackBufferColor(const Math::Color& col)
	{ m_clearColor = col; }

	// �f�o�C�X�擾
	inline ID3D12Device14* GetDevice() const
	{ return m_cpDevice.Get(); }

	// �R�}���h���X�g�擾
	inline ID3D12GraphicsCommandList10* GetCmdList() const
	{ return m_cpCmdList.Get(); }

	// �o�b�N�o�b�t�@�擾
	inline std::vector<ComPtr<ID3D12Resource>> GetBackBuffers()
	{ return m_cpBackBuffers; }

	// RTV�q�[�v�擾
	inline RTVHeap* GetRTVHeap() const
	{ return m_upRTVHeap.get(); }

	// CBVSRVUAV�q�[�v�擾
	inline CBVSRVUAVHeap* GetCBVSRVUAVHeap() const
	{ return m_upCBVSRVUAVHeap.get(); }

	// DSV�q�[�v�擾
	inline DSVHeap* GetDSVHeap() const
	{ return m_upDSVHeap.get(); }

	// ImGui�q�[�v�擾
	inline ImGuiHeap* GetImGuiHeap() const
	{ return m_upImGuiHeap.get(); }

	// �萔�o�b�t�@�A���P�[�^�[�擾
	inline ConstantBufferAllocator* GetConstantBufferAllocator() const
	{ return m_upCBufferAllocator.get(); }

	// �[�x�o�b�t�@�擾
	inline DepthStencil* GetDepthStencil() const
	{ return m_upDepthStencil.get(); }

	// �e�p�[�x�o�b�t�@�擾
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
	bool CreateSwapChain(HWND hWnd, int width, int height);

	// �����_�[�^�[�Q�b�g�r���[�쐬
	bool CreateRTV();

	// �t�F���X�쐬
	bool CreateFence();

	//----------------------------------------------------------------
	// �f�o�C�X�֘A
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

	// �萔�o�b�t�@�A���P�[�^�[
	std::unique_ptr<ConstantBufferAllocator> m_upCBufferAllocator = nullptr;

	// �[�x�o�b�t�@
	std::unique_ptr<DepthStencil> m_upDepthStencil = nullptr;
	std::unique_ptr<DepthStencil> m_upLightDepthStencil = nullptr;

	// �o�b�N�o�b�t�@�F
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