#pragma once

class Camera
{
public:
	Camera() { SetProjMatrix(60.0f); }
	~Camera() {}

	/// <summary>
	/// �J�������V�F�[�_�[�ɃZ�b�g
	/// </summary>
	void SetToShader() const;

	/// <summary>
	/// �r���[�s����Z�b�g
	/// </summary>
	/// <param name="mView">�ݒ肷��r���[�s��</param>
	inline void SetViewMatrix(const Math::Matrix& mView)
	{ m_mView = mView; }

	/// <summary>
	/// ��������v���W�F�N�V�����s����v�Z���ăZ�b�g
	/// </summary>
	/// <param name="fov">����p</param>
	/// <param name="minRange">�j�A�N���b�v</param>
	/// <param name="maxRange">�t�@�[�N���b�v</param>
	/// <param name="aspectRatio">�A�X�y�N�g��</param>
	void SetProjMatrix(float fov, float minRange = 0.01f, float maxRange = 500.0f, float aspectRatio = 16.0f / 9.0f);
	/// <summary>
	/// �v���W�F�N�V�����s����Z�b�g
	/// </summary>
	/// <param name="mProj">�ݒ肷��v���W�F�N�V�����s��</param>
	void SetProjMatrix(const DirectX::SimpleMath::Matrix& mProj);
	
	/// <summary>
	/// �N���C�A���g���W����3D���[���h���W�����߂郌�C���𐶐�
	/// </summary>
	/// <param name="clientPos">�}�E�X���W</param>
	/// <param name="rayPos">���C�̈ʒu</param>
	/// <param name="rayDir">���C�̕���</param>
	/// <param name="rayRange">���C�̒���</param>
	void GenerateRayInfoFromClient(const POINT& clientPos, Math::Vector3& rayPos, Math::Vector3& rayDir, float& rayRange) const;

	/// <summary>
	/// �r���[�s���Ԃ�
	/// </summary>
	/// <returns>�r���[�s��</returns>
	inline const Math::Matrix& GetViewMatrix() const 
	{ return m_mView; }

	/// <summary>
	/// �v���W�F�N�V�����s���Ԃ�
	/// </summary>
	/// <returns>�v���W�F�N�V�����s��</returns>
	inline const Math::Matrix& GetProjMatrix() const
	{ return m_mProj; }

private:
	Math::Matrix m_mView;

	Math::Matrix m_mProj;
};