#pragma once

#include "MeshData/MeshData.h"

class Texture;

struct MeshFace
{
	UINT Idx[3];
};

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

class Mesh
{
public:
	// ���b�V���쐬
	void Create(const std::vector<MeshVertex>& vertices,
		const std::vector<MeshFace>& faces,const Material& material);

	// �C���X�^���X�`��
	void DrawInstanced(UINT vertexCount) const;

	// �}�e���A���擾
	inline const Material& GetMaterial()const 
	{ return m_material; }

	// �C���X�^���X���擾
	inline UINT GetInstanceCount()const 
	{ return m_instanceCount; }

	// �����s���E�{�b�N�X�擾
	inline const DirectX::BoundingBox& GetBoundingBox() const
	{ return m_aabb; }
	// ���E���擾
	inline const DirectX::BoundingSphere& GetBoundingSphere() const 
	{ return m_bs; }

	inline const std::vector<Math::Vector3>& GetPositions()const
	{ return m_positions; }

	inline const std::vector<MeshFace>& GetFaces()const 
	{ return m_faces; }

private:
	ComPtr<ID3D12Resource> m_cpVBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_vbView = {};

	ComPtr<ID3D12Resource> m_cpIBuffer = nullptr;
	D3D12_INDEX_BUFFER_VIEW m_ibView = {};

	UINT m_instanceCount = 0;
	Material m_material;

	// ���E�f�[�^
	DirectX::BoundingBox m_aabb;
	DirectX::BoundingSphere m_bs;

	// ���W�݂̂̔z��
	std::vector<Math::Vector3> m_positions;
	std::vector<MeshFace> m_faces;

};