#pragma once

#include "ModelShader/ModelShader.h"
#include "SpriteShader/SpriteShader.h"

class ShaderManager
{
public:
	void Init();

	void SetCamera(const Math::Matrix& view,const Math::Matrix& proj);
	void WriteCamera() const;

	ModelShader m_modelShader;
	SpriteShader m_spriteShader;

private:
	ConstantBufferData::Camera m_cbCamera;

public:
	static ShaderManager& GetInstance()
	{
		static ShaderManager instance;
		return instance;
	}
};
