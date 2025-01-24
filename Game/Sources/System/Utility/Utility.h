#pragma once

// ComPtr
using Microsoft::WRL::ComPtr;

// Direct3D D3D12 Agility SDK
extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 714; }
extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = u8".\\D3D12\\"; }

// SimpleMath
namespace Math = DirectX::SimpleMath;

// �t�@�C���p�X����e�f�B���N�g���܂ł̃p�X�擾
inline std::string GetDirFromPath(const std::string& path)
{
	const std::string::size_type pos = path.find_last_of('/');
	return (pos == std::string::npos) ? std::string() : path.substr(0, pos + 1);
}

inline float EaseInOutSine(float Progress)
{
	return float(-(std::cos(M_PI * Progress) - 1.0f) / 2.0f);
}

// ��ʃT�C�Y
constexpr int window_width = 1280;
constexpr int window_height = 720;

// �V���h�E�}�b�v�𑜓x
constexpr int shadow_definition = 2048;