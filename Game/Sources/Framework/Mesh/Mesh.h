#pragma once

#include "MeshData/MeshData.h"

struct MeshFace
{
	UINT Idx[3];
};

struct Material
{
	std::string Name; // �}�e���A����

	std::shared_ptr<Texture> spBaseColorTex; // ��{�F�e�N�X�`��
	Math::Vector4 BaseColor = { 1,1,1,1 }; // ��{�F�X�P�[�����O�W��

	std::shared_ptr<Texture> spMetallicRoughnessTex; // ������ �e��
	float Metallic = 0.0f; // �������̃X�P�[�����O�W��
	float Roughness = 1.0f; // �e���̃X�P�[�����O�W��

	std::shared_ptr<Texture> spEmissiveTex; // ���Ȕ����e�N�X�`��
	Math::Vector3 Emissive = { 0,0,0 }; // ���Ȕ����̃X�P�[�����O�W��

	std::shared_ptr<Texture> spNormalTex; // �@���e�N�X�`��
};

class Mesh
{
public:
	// ���b�V���쐬
	void Create(const std::vector<MeshVertex>& vertices,
		const std::vector<MeshFace>& faces,const Material& material);

	// �C���X�^���X�`��
	void DrawInstanced(UINT vertexCount) const;

	inline const Material& GetMaterial()const 
	{ return m_material; }

private:
	ComPtr<ID3D12Resource> m_cpVBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_vbView = {};

	ComPtr<ID3D12Resource> m_cpIBuffer = nullptr;
	D3D12_INDEX_BUFFER_VIEW m_ibView = {};

	Material m_material;
};