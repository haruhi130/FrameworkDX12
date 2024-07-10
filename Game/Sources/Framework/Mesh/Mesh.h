#pragma once

#include "MeshData/MeshData.h"

class Texture;

struct MeshFace
{
	UINT Idx[3];
};

struct Material
{
	std::string Name; // マテリアル名

	std::shared_ptr<Texture> spBaseColorTex; // 基本色テクスチャ
	Math::Vector4 BaseColor = { 1,1,1,1 }; // 基本色スケーリング係数

	std::shared_ptr<Texture> spMetallicRoughnessTex; // 金属製 粗さ
	float Metallic = 0.0f; // 金属製のスケーリング係数
	float Roughness = 1.0f; // 粗さのスケーリング係数

	std::shared_ptr<Texture> spEmissiveTex; // 自己発光テクスチャ
	Math::Vector3 Emissive = { 0,0,0 }; // 自己発光のスケーリング係数

	std::shared_ptr<Texture> spNormalTex; // 法線テクスチャ
};

class Mesh
{
public:
	// メッシュ作成
	void Create(const std::vector<MeshVertex>& vertices,
		const std::vector<MeshFace>& faces,const Material& material);

	// インスタンス描画
	void DrawInstanced(UINT vertexCount) const;

	// マテリアル取得
	inline const Material& GetMaterial()const 
	{ return m_material; }

	// インスタンス数取得
	inline UINT GetInstanceCount()const 
	{ return m_instanceCount; }

private:
	ComPtr<ID3D12Resource> m_cpVBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_vbView = {};

	ComPtr<ID3D12Resource> m_cpIBuffer = nullptr;
	D3D12_INDEX_BUFFER_VIEW m_ibView = {};

	UINT m_instanceCount = 0;
	Material m_material;
};