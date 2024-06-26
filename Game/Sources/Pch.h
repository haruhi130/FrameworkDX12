#pragma once
//==============================================
// プリコンパイル済みヘッダー
//==============================================
#pragma comment(lib,"winmm.lib")
#define NOMINMAX

#include <sdkddkver.h>
#include <Windows.h>

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

#include <d3d12.h>
#include <dxgi1_6.h>


#include "System/System.h"