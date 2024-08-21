#pragma once

//=================================================
// ���b�V���̓����蔻�茋��
//=================================================
struct CollisionMeshResult
{
	DirectX::XMVECTOR hitPos = {};// �����������W
	DirectX::XMVECTOR hitDir = {};// �Ώۂւ̕����x�N�g��
	float overlapDistance = 0.0f; // �d�Ȃ��Ă��鋗��
	bool isHit = false;				// �����������ǂ���
};

// ���C�̓����蔻��
//bool PolygonsIntersect(const KdPolygon& poly, const DirectX::XMVECTOR& rayPos, const DirectX::XMVECTOR& rayDir, float rayRange,
//	const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult = nullptr);
bool MeshIntersect(const Mesh& mesh, const DirectX::XMVECTOR& rayPos, const DirectX::XMVECTOR& rayDir, float rayRange,
	const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult = nullptr);

// �X�t�B�A�̓����蔻��
//bool PolygonsIntersect(const KdPolygon& poly, const DirectX::BoundingSphere& sphere,
//	const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult = nullptr);
bool MeshIntersect(const Mesh& mesh, const DirectX::BoundingSphere& sphere,
	const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult = nullptr);

// �_ vs �O�p�`�ʂƂ̍ŋߐړ_�����߂�
void PointToTriangle(const DirectX::XMVECTOR& point, const DirectX::XMVECTOR& v1,
	const DirectX::XMVECTOR& v2, const DirectX::XMVECTOR& v3, DirectX::XMVECTOR& nearestPoint);