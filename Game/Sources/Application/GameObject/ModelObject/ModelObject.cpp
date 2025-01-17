#include "ModelObject.h"

void ModelObject::Draw()
{
	if (!m_spModel) { return; }
	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld);
}

void ModelObject::DrawShadow()
{
	if (!m_spModel) { return; }
	ShaderManager::GetInstance().m_modelShader.DrawModel(*m_spModel, m_mWorld, false);
}
