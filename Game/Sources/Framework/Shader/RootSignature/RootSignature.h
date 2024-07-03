#pragma once

enum class RangeType
{
	CBV,
	SRV,
	UAV,
};

enum class TextureAddressMode
{
	Wrap,
	Clamp,
};

enum class D3D12Filter
{
	Point,
	Linear,
};

class RootSignature
{
public:
	// ���[�g�V�O�l�`���쐬
	void Create(const std::vector<RangeType>& rangeTypes, UINT& cbvCount);

	// ���[�g�V�O�l�`���擾
	inline ID3D12RootSignature* GetRootSignature() const
	{ return m_cpRootSignature.Get(); }

private:
	// �����W�쐬
	void CreateRange(D3D12_DESCRIPTOR_RANGE& pRange, RangeType type, int count);

	// �T���v���[�쐬
	void CreateStaticSampler(D3D12_STATIC_SAMPLER_DESC& samplerDesc, TextureAddressMode mode,
		D3D12Filter filter, int count);

	ComPtr<ID3DBlob> m_cpRootBlob = nullptr;
	ComPtr<ID3D12RootSignature> m_cpRootSignature = nullptr;
};