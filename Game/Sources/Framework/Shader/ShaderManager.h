#pragma once

#include "ModelShader/ModelShader.h"
#include "SpriteShader/SpriteShader.h"
#include "PostProcessShader/PostProcessShader.h"

class ShaderManager
{
public:
	// �V�F�[�_�[������
	bool Init();

	void PreUpdate();

	//-------------------------------------------------
	// �J�����ݒ�
	//-------------------------------------------------
	void SetCamera(const Math::Matrix& view,const Math::Matrix& proj);
	void WriteCBCamera() const;

	const ConstantBufferData::CameraInfo& GetCBCamera() const
	{ return m_cbCamera; }

	//-------------------------------------------------
	// ���C�g�ݒ�
	//-------------------------------------------------
	void SetIsUseLight(bool isUseLight);
	void SetDirectionalLight(const Math::Vector3& dir, const Math::Vector3& color = {1,1,1});
	void SetAmbientLight(float pow);
	void SetDirLightShadowArea(const Math::Vector2& area, float height);

	void WriteCBLight() const;
	void WriteCBShadowArea(const Math::Matrix& proj, float dirLightHeight);
	void WriteLightParams();
	void WritePointLight(const std::list<ConstantBufferData::PointLight>& pointLights);
	void WriteSpotLight(const std::list<ConstantBufferData::SpotLight>& spotLights);

	const ConstantBufferData::LightInfo& GetCBLight() const
	{ return m_cbLight; }

	//-------------------------------------------------
	// �����ǉ�
	//-------------------------------------------------
	// �_���ǉ�
	void AddPointLight(const Math::Vector3& color, float radius, const Math::Vector3& pos, bool isBright);
	void AddPointLight(const ConstantBufferData::PointLight& pointLight);

	// �W�����ǉ�
	void AddSpotLight(const Math::Vector3& color, float range, const Math::Vector3& pos, float angle, const Math::Vector3& dir,bool isEnable = true);
	void AddSpotLight(const ConstantBufferData::SpotLight& spotLight);

	//-------------------------------------------------
	// �V�F�[�_�[
	//-------------------------------------------------
	ModelShader			m_modelShader;			// 3D���f���`��V�F�[�_�[
	SpriteShader		m_spriteShader;			// 2D�e�N�X�`���`��V�F�[�_�[
	PostProcessShader	m_postProcessShader;	// �|�X�g�v���Z�X���H�V�F�[�_�[

private:
	// �J�����萔�o�b�t�@
	ConstantBufferData::CameraInfo m_cbCamera;

	// ���C�g�萔�o�b�t�@
	ConstantBufferData::LightInfo m_cbLight;

	// �e�`��͈�
	Math::Matrix m_shadowProj;
	// ���s�����̍���
	float m_dirLightHeight = 0.0f;

	// �|�C���g���C�g���X�g
	std::list<ConstantBufferData::PointLight> m_pointLights;
	// �X�|�b�g���C�g���X�g
	std::list<ConstantBufferData::SpotLight> m_spotLights;

public:
	static ShaderManager& GetInstance()
	{
		static ShaderManager instance;
		return instance;
	}
};
