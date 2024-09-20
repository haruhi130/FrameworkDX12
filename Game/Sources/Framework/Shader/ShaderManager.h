#pragma once

#include "ModelShader/ModelShader.h"

class ShaderManager
{
public:
	void Init();

	void SetCamera(const Math::Matrix& view,const Math::Matrix& proj);
	void WriteCamera() const;

	ModelShader m_modelShader;

private:
	ConstantBufferData::Camera m_cbCamera;

public:
	static ShaderManager& GetInstance()
	{
		static ShaderManager instance;
		return instance;
	}
};
