#pragma once
//==============================================
// プリコンパイル済みヘッダー
//==============================================
#pragma comment(lib,"winmm.lib")
#define NOMINMAX

#include <sdkddkver.h>
#include <Windows.h>
#include <stdio.h>

#include <wrl/client.h>

// C Library
#include <cassert>

//==============================================
// STL
//==============================================
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <future>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

// multimedia
#include <mmsystem.h>

//==============================================
// Direct3D12
//==============================================
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#include <DirectXCollision.h>

// DirectX Tool Kit
#pragma comment(lib,"DirectXTK12.lib")
#include <SimpleMath.h>
#include <Audio.h>

// DirectX Tex
#pragma comment(lib,"DirectXTex.lib")
#include <DirectXTex.h>

//==============================================
// ImGui
//==============================================
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

//==============================================
// Effekseer
//==============================================
#pragma comment(lib,"Effekseer.lib")
#pragma comment(lib,"EffekseerRendererCommon.lib")
#pragma comment(lib,"EffekseerRendererDX12.lib")
#pragma comment(lib,"EffekseerRendererLLGI.lib")
#pragma comment(lib,"LLGI.lib")

#include <Effekseer/Effekseer.h>
#include <EffekseerRendererDX12/EffekseerRendererDX12.h>

//==============================================
// 文字コード変換
//==============================================
#include <strconv.h>

// System
#include "System/System.h"