#pragma once

#include "MeshData/MeshData.h"

class Texture;

// メッシュ面情報
struct MeshFace
{
	UINT Idx[3];
};

// メッシュマテリアル情報
struct Material
{
	std::string Name = ""; // マテリアル名

	std::shared_ptr<Texture> spBaseColorTex = nullptr; // 基本色テクスチャ
	Math::Vector4 BaseColor = { 1,1,1,1 }; // 基本色スケーリング係数

	std::shared_ptr<Texture> spMetallicRoughnessTex = nullptr; // 金属製 粗さ
	float Metallic = 0.0f; // 金属製のスケーリング係数
	float Roughness = 1.0f; // 粗さのスケーリング係数

	std::shared_ptr<Texture> spEmissiveTex = nullptr; // 自己発光テクスチャ
	Math::Vector3 Emissive = { 0,0,0 }; // 自己発光のスケーリング係数

	std::shared_ptr<Texture> spNormalTex = nullptr; // 法線テクスチャ
};

// メッシュサブセット情報
struct MeshSubset
{
	UINT MaterialNo = 0;
	UINT FaceStart = 0;
	UINT FaceCount = 0;
};

class Mesh
{
public:
	// バッファセット
	void SetToDevice() const;

	// モデル用メッシュ作成
	void Create(const std::vector<MeshVertex>& vertices,
		const std::vector<MeshFace>& faces, const std::vector<MeshSubset>& subsets, bool isSkinMesh);

	// 画像用メッシュ作成
	void Create(const Texture& tex ,const Math::Vector2& pos, const Math::Rectangle* rect = nullptr, const Math::Vector2& pivot = { 0.5f,0.5f });

	// サブセット描画
	void DrawSubset(int subsetNo)const;

	// インスタンス描画
	void DrawIndexed(UINT vertexCount = 6) const;

	// サブセット取得
	inline const std::vector<MeshSubset>& GetSubsets() const 
	{ return m_subsets; }

	// 軸平行境界ボックス取得
	inline const DirectX::BoundingBox& GetBoundingBox() const
	{ return m_aabb; }
	// 境界球取得
	inline const DirectX::BoundingSphere& GetBoundingSphere() const 
	{ return m_bs; }

	// 座標配列取得
	inline const std::vector<Math::Vector3>& GetPositions() const
	{ return m_positions; }

	// 面配列取得
	inline const std::vector<MeshFace>& GetFaces() const 
	{ return m_faces; }

private:
	// 頂点バッファ
	ComPtr<ID3D12Resource> m_cpVBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_vbView = {};

	// インデックスバッファ
	ComPtr<ID3D12Resource> m_cpIBuffer = nullptr;
	D3D12_INDEX_BUFFER_VIEW m_ibView = {};

	// サブセット情報
	std::vector<MeshSubset> m_subsets;

	// 境界データ
	DirectX::BoundingBox m_aabb;
	DirectX::BoundingSphere m_bs;

	// 座標のみの配列
	std::vector<Math::Vector3> m_positions;
	// 面情報のみの配列
	std::vector<MeshFace> m_faces;

	// スキンメッシュ判別
	bool m_isSkinMesh = false;

};