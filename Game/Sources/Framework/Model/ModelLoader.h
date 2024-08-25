#pragma once

class ModelLoader
{
public:
	// モデルロード
	bool Load(std::string filePath, ModelData& model);

private:
	// モデル解析
	std::shared_ptr<Mesh> Parse(const aiScene* pScene, const aiMesh* pMesh,
		const aiMaterial* pMaterial, const std::string& dirPath);

	// マテリアル解析
	const Material ParseMaterial(const aiMaterial* pMaterial, const std::string& dirPath);
};