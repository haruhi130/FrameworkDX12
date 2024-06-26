#include "GraphicsDevice.h"

bool GraphicsDevice::Init()
{
	if (!CreateFactory())
	{
		assert(0 && "�t�@�N�g���[�쐬���s");
		return false;
	}

	if (!CreateDevice())
	{
		assert(0 && "�f�o�C�X�쐬���s");
		return false;
	}

	return true;
}

bool GraphicsDevice::CreateFactory()
{
	UINT dxgiFlags = 0;
	dxgiFlags |= DXGI_CREATE_FACTORY_DEBUG;
	
	auto result = CreateDXGIFactory2(dxgiFlags, IID_PPV_ARGS(m_cpDxgiFactory.ReleaseAndGetAddressOf()));
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool GraphicsDevice::CreateDevice()
{
	// �g�p����A�_�v�^�[
	IDXGIAdapter* pSelectAdapter = nullptr;
	// �A�_�v�^�[��
	std::vector<IDXGIAdapter*> pAdapters;
	std::vector<DXGI_ADAPTER_DESC> descs;

	// �g�p����PC��GPU�h���C�o�[���������i�[����
	for (UINT index = 0; 1; ++index)
	{
		pAdapters.push_back(nullptr);
		HRESULT hr = m_cpDxgiFactory->EnumAdapters(index, &pAdapters[index]);
	
		if (hr == DXGI_ERROR_NOT_FOUND) { break; }

		descs.push_back({});
		pAdapters[index]->GetDesc(&descs[index]);
	}

	GPUTier gpuTier = GPUTier::Kind;

	// �D��x�̍���GPU�h���C�o�[���g�p����
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

	// �t�B�[�`�����x����
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

	// Direct3D�f�o�C�X������
	D3D_FEATURE_LEVEL featureLevel;
	for (auto lv : featureLevels)
	{
		if(D3D12CreateDevice(nullptr,lv , IID_PPV_ARGS(m_cpDevice.ReleaseAndGetAddressOf()))== S_OK)
		featureLevel = lv;
		break;
	}

	return true;
}
