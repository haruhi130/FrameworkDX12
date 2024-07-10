#include "Model.h"
#include "ModelLoader.h"

bool ModelData::Load(const std::string& filePath)
{
	ModelLoader modelLoader;
	if (!modelLoader.Load(filePath, m_nodes))
	{
		assert(0 && "ƒ‚ƒfƒ‹ƒ[ƒh¸”s");
		return false;
	}

	return true;
}
