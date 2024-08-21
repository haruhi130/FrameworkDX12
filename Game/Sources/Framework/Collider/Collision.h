#pragma once

//=================================================
// メッシュの当たり判定結果
//=================================================
struct CollisionMeshResult
{
	DirectX::XMVECTOR hitPos = {};// 当たった座標
	DirectX::XMVECTOR hitDir = {};// 対象への方向ベクトル
	float overlapDistance = 0.0f; // 重なっている距離
	bool isHit = false;				// 当たったかどうか
};

// レイの当たり判定
//bool PolygonsIntersect(const KdPolygon& poly, const DirectX::XMVECTOR& rayPos, const DirectX::XMVECTOR& rayDir, float rayRange,
//	const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult = nullptr);
bool MeshIntersect(const Mesh& mesh, const DirectX::XMVECTOR& rayPos, const DirectX::XMVECTOR& rayDir, float rayRange,
	const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult = nullptr);

// スフィアの当たり判定
//bool PolygonsIntersect(const KdPolygon& poly, const DirectX::BoundingSphere& sphere,
//	const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult = nullptr);
bool MeshIntersect(const Mesh& mesh, const DirectX::BoundingSphere& sphere,
	const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult = nullptr);

// 点 vs 三角形面との最近接点を求める
void PointToTriangle(const DirectX::XMVECTOR& point, const DirectX::XMVECTOR& v1,
	const DirectX::XMVECTOR& v2, const DirectX::XMVECTOR& v3, DirectX::XMVECTOR& nearestPoint);