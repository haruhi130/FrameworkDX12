#include "Collision.h"
using namespace DirectX;

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ���C�̏����t�s�񉻂���
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// ���C�ƃ|���S���𔻒肷��ۂɑS�Ă̒��_���s��ړ�������ƃ|���S�����ɂ���ď����R�X�g���ς�邽�ߔ��ɕs����
// ���C�̏���1�����Ȃ����߃��C�������t�s�񉻂��鎖�ŏ����̈��艻�{1�x�����v�Z���s���Ȃ����ߍő�̌������ɂ��Ȃ�
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
static void InvertRayInfo(DirectX::XMVECTOR& rayPosInv, DirectX::XMVECTOR& rayDirInv, float& rayRangeInv, float& scaleInv,
	const DirectX::XMMATRIX& matrix, const DirectX::XMVECTOR& rayPos, const DirectX::XMVECTOR& rayDir, float rayRange)
{
	// �^�[�Q�b�g�̋t�s��Ń��C��ϊ�
	DirectX::XMMATRIX invMat = XMMatrixInverse(0, matrix);

	// ���C�̔���J�n�ʒu���t�ϊ�
	rayPosInv = XMVector3TransformCoord(rayPos, invMat);

	// ���C�̕������t�ϊ�
	rayDirInv = XMVector3TransformNormal(rayDir, invMat);

	// �g�嗦���t�ϊ�
	scaleInv = DirectX::XMVector3Length(rayDirInv).m128_f32[0];

	// ���C�̕����x�N�g���̒���=�g�嗦�Ŕ�����E������␳
	// ���t�s��Ɋg�k�������Ă���ƁA���C�̒������ς�邽��
	// ���C�������������W����̋����Ɋg�k�����f����Ă��܂��̂�
	// ����p�̍ő勗���ɂ��g�k�𔽉f�����Ă���
	rayRangeInv = rayRange * scaleInv;

	// ���p���C�Ƃ��Đ������������߂ɂ͒������P�łȂ���΂Ȃ�Ȃ��̂Ő��K��
	rayDirInv = DirectX::XMVector3Normalize(rayDirInv);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ���C�Ƃ̓����蔻�茋�ʂ����U���g�ɃZ�b�g����
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
static void SetRayResult(CollisionMeshResult& result, bool isHit, float closestDist,
	const DirectX::XMVECTOR& rayPos, const DirectX::XMVECTOR& rayDir, float rayRange)
{
	// ���U���g�Ɍ��ʂ��i�[
	result.isHit = isHit;

	result.hitPos = DirectX::XMVectorAdd(rayPos, DirectX::XMVectorScale(rayDir, closestDist));

	result.hitDir = DirectX::XMVectorScale(rayDir, -1);

	result.overlapDistance = rayRange - closestDist;
}

bool MeshIntersect(const Mesh& mesh, const DirectX::XMVECTOR& rayPos, const DirectX::XMVECTOR& rayDir, float rayRange, const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult)
{
	//--------------------------------------------------------
	// �u���[�h�t�F�C�Y
	// �@��r�I�y�ʂ�AABB vs ���C�Ȕ���ŁA
	// �@�����炩�Ƀq�b�g���Ȃ��ꍇ�́A����ȍ~�̔���𒆎~
	//--------------------------------------------------------
	{
		// AABB vs ���C
		float AABBdist = FLT_MAX;
		DirectX::BoundingBox aabb;
		mesh.GetBoundingBox().Transform(aabb, matrix);

		if (aabb.Intersects(rayPos, rayDir, AABBdist) == false) { return false; }

		// �ő勗���O�Ȃ�͈͊O�Ȃ̂Œ��~
		if (AABBdist > rayRange) { return false; }
	}

	//--------------------------------------------------------
	// ���C�̋t�s��
	//--------------------------------------------------------
	DirectX::XMVECTOR rayPosInv, rayDirInv;
	float rayRangeInv = 0;
	float scaleInv = 0;

	InvertRayInfo(rayPosInv, rayDirInv, rayRangeInv, scaleInv,
		matrix, rayPos, rayDir, rayRange);

	//--------------------------------------------------------
	// �i���[�t�F�C�Y
	// �@���C vs �S�Ă̖�
	//--------------------------------------------------------

	// �q�b�g����
	bool isHit = false;
	float closestDist = FLT_MAX;

	// DEBUG�r���h�ł����x���ێ����邽�߁A�ʕϐ��ɏE���Ă���
	const MeshFace* pFaces = &mesh.GetFaces()[0];
	auto& vertices = mesh.GetPositions();
	size_t faceNum = mesh.GetFaces().size();

	// �S�Ă̖�(�O�p�`)
	for (UINT faceIdx = 0; faceIdx < faceNum; ++faceIdx)
	{
		// �O�p�`���\������R�̒��_��Index
		const UINT* idx = pFaces[faceIdx].Idx;

		// ���C�ƎO�p�`�̔���
		float hitDist = FLT_MAX;
		if (!DirectX::TriangleTests::Intersects(rayPosInv, rayDirInv,
			vertices[idx[0]], vertices[idx[1]], vertices[idx[2]],
			hitDist))
		{
			continue;
		}

		// ���C�̔���͈͊O�Ȃ疳��
		if (hitDist > rayRangeInv) { continue; }

		// CollisionResult�����Ȃ猋�ʂ͊֌W�Ȃ��̂œ����������_�ŕԂ�
		if (!pResult) { return isHit; }

		// �ŒZ�����̍X�V���菈��
		closestDist = std::min(hitDist, closestDist);

		isHit = true;
	}

	if (pResult && isHit)
	{
		SetRayResult(*pResult, isHit, closestDist / scaleInv, rayPos, rayDir, rayRange);
	}

	return isHit;
}

// �X�t�B�A�����t�s��
static void InvertSphereInfo(DirectX::XMVECTOR& spherePosInv, DirectX::XMVECTOR& sphereScale, float& radiusSqr,
	const DirectX::XMMATRIX& matrix, const DirectX::BoundingSphere& sphere)
{
	// ���b�V���̋t�s��ŁA���̒��S���W��ϊ�(���b�V���̍��W�n�֕ϊ������)
	DirectX::XMMATRIX invMat = XMMatrixInverse(0, matrix);
	spherePosInv = XMVector3TransformCoord(XMLoadFloat3(&sphere.Center), invMat);

	// ���a�̓��(����̍������p)
	radiusSqr = sphere.Radius * sphere.Radius;	// ���a�̂Q��

	// �s��̊e���̊g�嗦���擾���Ă���
	sphereScale.m128_f32[0] = DirectX::XMVector3Length(matrix.r[0]).m128_f32[0];
	sphereScale.m128_f32[1] = DirectX::XMVector3Length(matrix.r[1]).m128_f32[0];
	sphereScale.m128_f32[2] = DirectX::XMVector3Length(matrix.r[2]).m128_f32[0];
	sphereScale.m128_f32[3] = 0;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// �X�t�B�A�ƃ|���S���̍ŋߐړ_�����ɐڐG���Ă��邩�ǂ����𔻒�
// ���̃|���S���̔���̊Ԃɓ�����Ȃ��ʒu�܂ŃX�t�B�A���ړ�������
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
static bool HitCheckAndPosUpdate(DirectX::XMVECTOR& finalPos, DirectX::XMVECTOR& finalHitPos,
	const DirectX::XMVECTOR& nearPoint, const DirectX::XMVECTOR& objScale, float radiusSqr, float sphereRadius)
{
	// �ŋߐړ_�����̒��S�@�x�N�g�������߂�
	DirectX::XMVECTOR vToCenter = finalPos - nearPoint;

	// XYZ�����ʁX�̑傫���Ŋg�k����Ă��Ԃ̏ꍇ�A�����ό`���Ă��ԂȂ��ߐ��m�Ȕ��a���킩��Ȃ��B
	// ������scale��������XYZ���̃X�P�[�����ϓ��ȍ��W�n�֕ϊ�����
	vToCenter *= objScale;

	// �ŋߐړ_�����a��2���艓���ꍇ�́A�Փ˂��Ă��Ȃ�
	if (DirectX::XMVector3LengthSq(vToCenter).m128_f32[0] > radiusSqr)
	{
		return false;
	}

	// �����߂��v�Z
	// �߂荞��ł���Ԃ�̃x�N�g�����v�Z
	DirectX::XMVECTOR vPush = DirectX::XMVector3Normalize(vToCenter);

	vPush *= sphereRadius - DirectX::XMVector3Length(vToCenter).m128_f32[0];

	// �g�k���l���������W�n�֖߂�
	vPush /= objScale;

	// ���̒��S���W���X�V
	finalPos += vPush;

	finalHitPos = nearPoint;

	return true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// �X�t�B�A�Ƃ̓����蔻�茋�ʂ����U���g�ɃZ�b�g����
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
static void SetSphereResult(CollisionMeshResult& result, bool isHit, const DirectX::XMVECTOR& hitPos,
	const DirectX::XMVECTOR& finalPos, const DirectX::XMVECTOR& beginPos)
{
	result.isHit = isHit;

	result.hitPos = hitPos;

	result.hitDir = DirectX::XMVectorSubtract(finalPos, beginPos);

	result.overlapDistance = DirectX::XMVector3Length(result.hitDir).m128_f32[0];

	result.hitDir = DirectX::XMVector3Normalize(result.hitDir);
}

// �X�t�B�A�΃��b�V���̏Փ˔���{��
bool MeshIntersect(const Mesh& mesh, const DirectX::BoundingSphere& sphere, const DirectX::XMMATRIX& matrix, CollisionMeshResult* pResult)
{
	//------------------------------------------
	// �u���[�h�t�F�C�Y
	// �@�������̂��߁A�܂��͋��E�{�b�N�X(AABB)�Ŕ���
	// �@���̒i�K�Ńq�b�g���Ă��Ȃ��Ȃ�A�ڍׂȔ��������K�v�Ȃ�
	//------------------------------------------
	{
		// ���b�V����AABB�����ɁA�s��ŕϊ�����AABB���쐬
		DirectX::BoundingBox aabb;
		mesh.GetBoundingBox().Transform(aabb, matrix);

		if (aabb.Intersects(sphere) == false) { return false; }
	}

	//------------------------------------------
	// �i���[�t�F�C�Y
	// �@���ƃ��b�V���Ƃ̏ڍה���
	//------------------------------------------

	// �P�ł��q�b�g������true
	bool isHit = false;

	// DEBUG�r���h�ł����x���ێ����邽�߁A�ʕϐ��ɏE���Ă���
	const auto* pFaces = &mesh.GetFaces()[0];
	auto faceNum = mesh.GetFaces().size();
	auto& vertices = mesh.GetPositions();

	DirectX::XMVECTOR finalHitPos = {};	// �����������W�̒��ł��Ō�̍��W
	DirectX::XMVECTOR finalPos = {};	// �e�ʂɉ�����čŏI�I�ɓ��B������W�F���肷�鋅�̒��S
	DirectX::XMVECTOR objScale = {};	// �^�[�Q�b�g�I�u�W�F�N�g�̊e���̊g�嗦
	float radiusSqr = 0.0f;
	InvertSphereInfo(finalPos, objScale, radiusSqr, matrix, sphere);

	// �S�Ă̖ʂƔ���
	// ������̓��b�V���̃��[�J����Ԃōs����
	for (UINT faceIdx = 0; faceIdx < faceNum; faceIdx++)
	{
		DirectX::XMVECTOR nearPoint;

		// �O�p�`���\������R�̒��_��Index
		const UINT* idx = pFaces[faceIdx].Idx;

		// �_ �� �O�p�` �̍ŋߐړ_�����߂�
		PointToTriangle(finalPos, vertices[idx[0]], vertices[idx[1]], vertices[idx[2]], nearPoint);

		// �������Ă��邩�ǂ����̔���ƍŏI���W�̍X�V
		isHit |= HitCheckAndPosUpdate(finalPos, finalHitPos, nearPoint, objScale, radiusSqr, sphere.Radius);

		// CollisionResult�����Ȃ猋�ʂ͊֌W�Ȃ��̂œ����������_�ŕԂ�
		if (!pResult && isHit) { return isHit; }
	}

	// ���U���g�Ɍ��ʂ��i�[
	if (pResult && isHit)
	{
		SetSphereResult(*pResult, isHit, XMVector3TransformCoord(finalHitPos, matrix),
			XMVector3TransformCoord(finalPos, matrix), XMLoadFloat3(&sphere.Center));
	}

	return isHit;
}

void PointToTriangle(const DirectX::XMVECTOR& p, const DirectX::XMVECTOR& a, const DirectX::XMVECTOR& b, const DirectX::XMVECTOR& c, DirectX::XMVECTOR& outPt)
{
	// ���Q�l:[����]�u�Q�[���v���O���~���O�̂��߂̃��A���^�C���Փ˔���v

	// p��a�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR ab = b - a;
	XMVECTOR ac = c - a;
	XMVECTOR ap = p - a;

	float d1 = XMVector3Dot(ab, ap).m128_f32[0];//ab.Dot(ap);
	float d2 = XMVector3Dot(ac, ap).m128_f32[0];//ac.Dot(ap);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		outPt = a;	// �d�S���W(1,0,0)
		return;
	}

	// p��b�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR bp = p - b;
	float d3 = XMVector3Dot(ab, bp).m128_f32[0];//ab.Dot(bp);
	float d4 = XMVector3Dot(ac, bp).m128_f32[0];//ac.Dot(bp);

	if (d3 >= 0.0f && d4 <= d3)
	{
		outPt = b;	// �d�S���W(0,1,0)
		return;
	}

	// p��ab�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����p��ab��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;

	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		outPt = a + ab * v;	// �d�S���W(1-v,v,0)
		return;
	}

	// p��c�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR cp = p - c;
	float d5 = XMVector3Dot(ab, cp).m128_f32[0];//ab.Dot(cp);
	float d6 = XMVector3Dot(ac, cp).m128_f32[0];;//ac.Dot(cp);

	if (d6 >= 0.0f && d5 <= d6)
	{
		outPt = c;	// �d�S���W(0,0,1)
		return;
	}

	// p��ac�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����p��ac��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;

	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		outPt = a + ac * w;	// �d�S���W(1-w,0,w)
		return;
	}

	// p��bc�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����p��bc��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;

	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		outPt = b + (c - b) * w;	// �d�S���W(0,1-w,w)
		return;
	}

	// p�͖ʗ̈�̒��ɂ���BQ�����̏d�S���W(u,v,w)��p���Čv�Z
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	outPt = a + ab * v + ac * w;	// = u*a + v*b + w*c, u = va*demon = 1.0f - v - w
}
