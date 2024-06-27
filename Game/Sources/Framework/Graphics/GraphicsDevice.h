#pragma once

class GraphicsDevice
{
public:
	// ������
	bool Init();

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