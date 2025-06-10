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
	/// �`��֌W�̏�����
	/// </summary>
	/// <param name="hWnd">�E�B���h�E�n���h��</param>
	/// <param name="width">��ʉ���</param>
	/// <param name="height">��ʏc��</param>
	/// <returns>�������ɐ���������true</returns>
	bool Init(HWND hWnd, int width, int height);

	/// <summary>
	/// �`��O����
	/// </summary>
	void Prepare();

	/// <summary>
	/// �X���b�v�`�F�[���ؑ�
	/// </summary>
	void ScreenFlip();

	/// <summary>
	/// �R�}���h�L���[�ҋ@
	/// </summary>
	void WaitForCommandQueue();

	/// <summary>
	/// �f�o�b�O���C���[�L����
	/// </summary>
	void EnableDebugLayer();

	/// <summary>
	/// �X�N���[�����W���烏�[���h���W�ւ̕ϊ�
	/// </summary>
	/// <param name="screenPos">�}�E�X���W</param>
	/// <param name="projZ">���s�����</param>
	/// <param name="dst">���ʊi�[</param>
	/// <param name="mCam">�r���[�s��</param>
	/// <param name="mProj">�v���W�F�N�V�����s��</param>
	void ClientToWorld(const POINT& screenPos, float projZ, Math::Vector3& dst, const Math::Matrix& mCam, const Math::Matrix& mProj);

	/// <summary>
	/// ���\�[�X�o���A�ؑ�
	/// </summary>
	/// <param name="pResource">�؂�ւ��郊�\�[�X</param>
	/// <param name="before">�֑ؑO�̃X�e�[�g</param>
	/// <param name="after">�ؑ֌�̃X�e�[�g</param>
	void SetResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES before,
		D3D12_RESOURCE_STATES after);

	/// <summary>
	/// �o�b�N�o�b�t�@�F�ύX
	/// </summary>
	/// <param name="col">�ύX�F</param>
	inline void SetBackBufferColor(const Math::Color& col)
	{ m_clearColor = col; }

	/// <summary>
	/// �f�o�C�X�擾
	/// </summary>
	/// <returns>�f�o�C�X</returns>
	inline ID3D12Device14* GetDevice() const
	{ return m_cpDevice.Get(); }

	/// <summary>
	/// �R�}���h���X�g�擾
	/// </summary>
	/// <returns>�R�}���h���X�g</returns>
	inline ID3D12GraphicsCommandList10* GetCmdList() const
	{ return m_cpCmdList.Get(); }

	/// <summary>
	/// �R�}���h�L���[�擾
	/// </summary>
	/// <returns>�R�}���h�L���[</returns>
	inline ID3D12CommandQueue* GetCmdQueue() const
	{ return m_cpCmdQueue.Get(); }

	/// <summary>
	/// �o�b�N�o�b�t�@�擾
	/// </summary>
	/// <returns>�o�b�N�o�b�t�@</returns>
	inline std::vector<ComPtr<ID3D12Resource>> GetBackBuffers()
	{ return m_cpBackBuffers; }

	/// <summary>
	/// RTV�q�[�v�擾
	/// </summary>
	/// <returns>RTV�q�[�v</returns>
	inline RTVHeap* GetRTVHeap() const
	{ return m_upRTVHeap.get(); }

	/// <summary>
	/// CBVSRVUAV�q�[�v�擾 
	/// </summary>
	/// <returns>CBVSRVUAV�q�[�v</returns>
	inline CBVSRVUAVHeap* GetCBVSRVUAVHeap() const
	{ return m_upCBVSRVUAVHeap.get(); }
	
	/// <summary>
	/// DSV�q�[�v�擾
	/// </summary>
	/// <returns> DSV�q�[�v</returns>
	inline DSVHeap* GetDSVHeap() const
	{ return m_upDSVHeap.get(); }

	/// <summary>
	/// ImGui�q�[�v�擾
	/// </summary>
	/// <returns>ImGui�q�[�v</returns>
	inline ImGuiHeap* GetImGuiHeap() const
	{ return m_upImGuiHeap.get(); }

	/// <summary>
	/// �萔�o�b�t�@�A���P�[�^�[�擾
	/// </summary>
	/// <returns>�萔�o�b�t�@�A���P�[�^�[</returns>
	inline ConstantBufferAllocator* GetConstantBufferAllocator() const
	{ return m_upCBufferAllocator.get(); }

	/// <summary>
	/// �[�x�o�b�t�@�擾
	/// </summary>
	/// <returns>�[�x�o�b�t�@</returns>
	inline DepthStencil* GetDepthStencil() const
	{ return m_upDepthStencil.get(); }

	/// <summary>
	/// �e�p�[�x�o�b�t�@�擾
	/// </summary>
	/// <returns>�e�p�[�x�o�b�t�@</returns>
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
	/// �t�@�N�g���[�쐬
	/// </summary>
	/// <returns>����������true</returns>
	bool CreateFactory();

	/// <summary>
	/// �f�o�C�X�쐬
	/// </summary>
	/// <returns>����������true</returns>
	bool CreateDevice();
	
	/// <summary>
	/// �R�}���h�A���P�[�^�[�쐬
	/// </summary>
	/// <returns>����������true</returns>
	bool CreateCommandAllocator();

	/// <summary>
	/// �R�}���h���X�g�쐬
	/// </summary>
	/// <returns>����������true</returns>
	bool CreateCommandList();

	/// <summary>
	/// �R�}���h�L���[�쐬
	/// </summary>
	/// <returns>����������true</returns>
	bool CreateCommandQueue();

	/// <summary>
	/// �X���b�v�`�F�[���쐬
	/// </summary>
	/// <param name="hWnd">�E�B���h�E�n���h��</param>
	/// <param name="width">��ʉ���</param>
	/// <param name="height">��ʏc��</param>
	/// <returns>����������true</returns>
	bool CreateSwapChain(HWND hWnd, int width, int height);

	/// <summary>
	/// �����_�[�^�[�Q�b�g�r���[�쐬
	/// </summary>
	/// <returns>����������true</returns>
	bool CreateRTV();

	/// <summary>
	/// �t�F���X�쐬
	/// </summary>
	/// <returns>����������true</returns>
	bool CreateFence();

	//----------------------------------------------------------------
	// �f�o�C�X
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