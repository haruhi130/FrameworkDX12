#pragma once
enum class HeapType
{
	CBVSRVUAV = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	SAMPLER = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
	RTV = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
	DSV = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
};

template<typename type>
class BaseHeap
{
public:
	BaseHeap(){}
	virtual ~BaseHeap(){}

	// ヒープ作成
	inline bool Create(HeapType heapType, type useCount)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(heapType);
		heapDesc.NodeMask = 0;
		heapDesc.NumDescriptors = ComputUseCount(useCount);
		heapDesc.Flags = heapType == HeapType::CBVSRVUAV ?
			D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		auto result = GraphicsDevice::GetInstance().GetDevice()->
			CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_cpHeap.ReleaseAndGetAddressOf()));
		if (FAILED(result))
		{
			return false;
		}

		m_useCount = useCount;
		m_incrementSize = GraphicsDevice::GetInstance().GetDevice()->GetDescriptorHandleIncrementSize(
			static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(heapType));

		return true;
	}

	// CPU側アドレス取得
	inline virtual const D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(int number)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = m_cpHeap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += (UINT64)m_incrementSize * number;
		return handle;
	}

	// GPU側アドレス取得
	inline virtual const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(int number)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE handle = m_cpHeap->GetGPUDescriptorHandleForHeapStart();
		handle.ptr += (UINT64)m_incrementSize * number;
		return handle;
	}

	inline ComPtr<ID3D12DescriptorHeap> GetHeap()
	{ return m_cpHeap; }

protected:
	ComPtr<ID3D12DescriptorHeap> m_cpHeap = nullptr;
	type m_useCount = {};
	int m_incrementSize = 0;
	int m_nextRegistNumber = 0;

private:
	inline UINT ComputUseCount(UINT useCount)
	{
		return useCount;
	}

    inline UINT ComputUseCount(Math::Vector3 useCount)
	{
		return (UINT)(useCount.x + useCount.y + useCount.z);
	}
};