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

// DirectX Tex
#pragma comment(lib,"DirectXTex.lib")
#include <DirectXTex.h>

// assimp
#ifdef _DEBUG
#pragma comment(lib,"assimp-vc143-mtd.lib")
#else
#pragma comment(lib,"assimp-vc143-mt.lib")
#endif

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

// XAudio2
#pragma comment(lib,"xaudio2.lib")
#include <xaudio2.h>

// multimedia
#include <mmsystem.h>

#include "System/System.h"