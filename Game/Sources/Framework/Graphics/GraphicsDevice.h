#pragma once

class GraphicsDevice
{
public:
	// ������
	bool Init();

private:
	// �t�@�N�g���[�쐬
	bool CreateFactory();

	// �f�o�C�X�쐬
	bool CreateDevice();

	enum class GPUTier
	{
		NVIDIA,
		Amd,
		Intel,
		Arm,
		Qualcomm,
		Kind,
	};

	ComPtr<ID3D12Device8> m_cpDevice = nullptr;
	ComPtr<IDXGIFactory6> m_cpDxgiFactory = nullptr;
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