#pragma once

class ModelLoader
{
public:
	// ���f�����[�h
	bool Load(std::string filePath, std::vector<ModelData::Node>& nodes);

private:
	// ���f�����
	std::shared_ptr<Mesh> Parse(const aiScene* pScene, const aiMesh* pMesh,
		const aiMaterial* pMaterial, const std::string& dirPath);

	// �}�e���A�����
	const Material ParseMaterial(const aiMaterial* pMaterial, const std::string& dirPath);

};