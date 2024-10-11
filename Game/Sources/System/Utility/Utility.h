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

// ��ʃT�C�Y
const int windowWidth = 1280;
const int windowHeight = 720;