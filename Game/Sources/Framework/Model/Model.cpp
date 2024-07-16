#include "Model.h"
#include "ModelLoader.h"

bool ModelData::Load(const std::string& filePath)
{
	if (filePath.empty()) { return false; }

	ModelLoader modelLoader;
	if (!modelLoader.Load(filePath, *this))
	{
		assert(0 && "ÉÇÉfÉãÉçÅ[Éhé∏îs");
		return false;
	}

	return true;
}

const std::shared_ptr<AnimationData> ModelData::GetAnimation(const std::string& animeName) const
{
	for (auto&& anime : m_spAnimations)
	{
		if (anime->m_name == animeName)
		{
			return anime;
		}
	}

	return nullptr;
}

const std::shared_ptr<AnimationData> ModelData::GetAnimation(UINT index) const
{
	return index >= m_spAnimations.size() ? nullptr : m_spAnimations[index];
}
