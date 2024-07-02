#pragma once

// ComPtr
using Microsoft::WRL::ComPtr;

// Direct3D D3D12 Agility SDK
extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 714; }
extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = u8".\\D3D12\\"; }

// SimpleMath
namespace Math = DirectX::SimpleMath;