#pragma once

#include "ModelShader/ModelShader.h"
#include "SpriteShader/SpriteShader.h"
#include "PostProcessShader/PostProcessShader.h"

class ShaderManager
{
public:
	// �V�F�[�_�[������
	bool Init();

	//-------------------------------------------------
	// �J�����ݒ�
	// �J��������ݒ�
	void SetCamera(const Math::Matrix& view,const Math::Matrix& proj);
	// �ݒ肵���J���������V�F�[�_�[�֓]��
	void WriteCBCamera() const;

	const ConstantBufferData::CameraInfo& GetCBCamera() const
	{ return m_cbCamera; }

	//-------------------------------------------------
	// ���C�g�ݒ�
	// �A�e�̗L����ݒ�E�]��
	void SetIsUseLight(bool isUseLight);
	// ���s���̕����ƐF��ݒ�
	void SetDirectionalLight(const Math::Vector3& dir, const Math::Vector3& color);
	// �����̋�����ݒ�
	void SetAmbientLight(float pow);

	void SetDirLightShadowArea(const Math::Vector2& area, float height);

	// �ݒ肵�����C�g�����V�F�[�_�[�֓]��
	void WriteCBLight() const;

	void WriteCBShadowArea(const Math::Matrix& proj, float dirLightHeight);
	
	void WriteLightParams();

	//-------------------------------------------------
	// �V�F�[�_�[
	//-------------------------------------------------
	ModelShader m_modelShader;
	SpriteShader m_spriteShader;
	PostProcessShader m_postProcessShader;

private:
	// �J�����萔�o�b�t�@
	ConstantBufferData::CameraInfo m_cbCamera;

	// ���C�g�萔�o�b�t�@
	ConstantBufferData::LightInfo m_cbLight;

	// �e�`��͈�
	Math::Matrix m_shadowProj;
	// ���C�g�̋^������
	float m_dirLightHeight = 0.0f;

public:
	static ShaderManager& GetInstance()
	{
		static ShaderManager instance;
		return instance;
	}
};
