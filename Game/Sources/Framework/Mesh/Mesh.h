#pragma once

#include "MeshData/MeshData.h"

class Texture;

// ���b�V���ʏ��
struct MeshFace
{
	UINT Idx[3];
};

// ���b�V���}�e���A�����
struct Material
{
	std::string Name = ""; // �}�e���A����

	std::shared_ptr<Texture> spBaseColorTex = nullptr; // ��{�F�e�N�X�`��
	Math::Vector4 BaseColor = { 1,1,1,1 }; // ��{�F�X�P�[�����O�W��

	std::shared_ptr<Texture> spMetallicRoughnessTex = nullptr; // ������ �e��
	float Metallic = 0.0f; // �������̃X�P�[�����O�W��
	float Roughness = 1.0f; // �e���̃X�P�[�����O�W��

	std::shared_ptr<Texture> spEmissiveTex = nullptr; // ���Ȕ����e�N�X�`��
	Math::Vector3 Emissive = { 0,0,0 }; // ���Ȕ����̃X�P�[�����O�W��

	std::shared_ptr<Texture> spNormalTex = nullptr; // �@���e�N�X�`��
};

// ���b�V���T�u�Z�b�g���
struct MeshSubset
{
	UINT MaterialNo = 0;
	UINT FaceStart = 0;
	UINT FaceCount = 0;
};

class Mesh
{
public:
	// �o�b�t�@�Z�b�g
	void SetToDevice() const;

	// ���f���p���b�V���쐬
	void Create(const std::vector<MeshVertex>& vertices,
		const std::vector<MeshFace>& faces, const std::vector<MeshSubset>& subsets, bool isSkinMesh);

	// �摜�p���b�V���쐬
	void Create(const Texture& tex ,const Math::Vector2& pos, const Math::Rectangle* rect = nullptr, const Math::Vector2& pivot = { 0.5f,0.5f });

	// �T�u�Z�b�g�`��
	void DrawSubset(int subsetNo)const;

	// �C���X�^���X�`��
	void DrawIndexed(UINT vertexCount = 6) const;

	// �T�u�Z�b�g�擾
	inline const std::vector<MeshSubset>& GetSubsets() const 
	{ return m_subsets; }

	// �����s���E�{�b�N�X�擾
	inline const DirectX::BoundingBox& GetBoundingBox() const
	{ return m_aabb; }
	// ���E���擾
	inline const DirectX::BoundingSphere& GetBoundingSphere() const 
	{ return m_bs; }

	// ���W�z��擾
	inline const std::vector<Math::Vector3>& GetPositions() const
	{ return m_positions; }

	// �ʔz��擾
	inline const std::vector<MeshFace>& GetFaces() const 
	{ return m_faces; }

private:
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> m_cpVBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_vbView = {};

	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> m_cpIBuffer = nullptr;
	D3D12_INDEX_BUFFER_VIEW m_ibView = {};

	// �T�u�Z�b�g���
	std::vector<MeshSubset> m_subsets;

	// ���E�f�[�^
	DirectX::BoundingBox m_aabb;
	DirectX::BoundingSphere m_bs;

	// ���W�݂̂̔z��
	std::vector<Math::Vector3> m_positions;
	// �ʏ��݂̂̔z��
	std::vector<MeshFace> m_faces;

	// �X�L�����b�V������
	bool m_isSkinMesh = false;

};