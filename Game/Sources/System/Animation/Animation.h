#pragma once

// �A�j���[�V�����L�[
struct AnimeKeyQuaternion
{
	float				m_time = 0;		// ����
	Math::Quaternion	m_quat;			// �N�H�[�^�j�I��
};

// �A�j���[�V�����L�[
struct AnimeKeyVector3
{
	float			m_time = 0;			// ����
	Math::Vector3	m_vec;				// 3D�x�N�g���f�[�^
};

// �A�j���[�V�����f�[�^
struct AnimationData
{
	std::string		m_name;				// �A�j���[�V������
	float			m_maxTime = 0;		// �A�j���[�V�����ő厞��

	struct Channel
	{
		int	m_nodeOffset = -1;	// �Ώۃ��f���m�[�h��Offset�l

		// �e�`�����l��
		std::vector<AnimeKeyVector3>	m_translations;	// �ʒu�L�[���X�g
		std::vector<AnimeKeyQuaternion>	m_rotations;	// ��]�L�[���X�g
		std::vector<AnimeKeyVector3>	m_scales;		// �g�k�L�[���X�g

		void Interpolate(Math::Matrix& rDst, float time);
		bool InterpolateTranslations(Math::Vector3& result, float time);
		bool InterpolateRotations(Math::Quaternion& result, float time);
		bool InterpolateScales(Math::Vector3& result, float time);
	};

	// �S�m�[�h�p�A�j���[�V�����f�[�^
	std::vector<Channel>	m_channels;
};

class Animator
{
public:
	// �A�j���[�V�����f�[�^�̎擾
	const std::shared_ptr<AnimationData> GetAnimationData()const { return m_spAnimation; }

	// �A�j���[�V�����̃Z�b�g
	inline void SetAnimation(const std::shared_ptr<AnimationData>& rData, const bool isLoop = true)
	{
		m_spAnimation = rData;
		m_isLoop = isLoop;

		m_time = 0.0f;
	}

	// �A�j���[�V�������I�����Ă��邩
	inline bool IsAnimationEnd() const
	{
		if (m_spAnimation == nullptr) { return true; }
		if (m_time >= m_spAnimation->m_maxTime) { return true; }

		return false;
	}

	// �A�j���[�V�����̍X�V
	void ProgressTime(std::vector<ModelWork::Node>& rNodes, float speed = 1.0f);

	// �A�j���[�V�����̃��Z�b�g
	inline void ResetAdvanceTime() 
	{ m_time = 0; }

	// ���݂̃A�j���[�V�������Ԃ��擾
	inline float GetNowAnimationTime()const
	{ return m_time; }

	// ���݂̃A�j���[�V�������Ԃ��Z�b�g
	inline void SetNowAnimationTime(float nowAnimationTime)
	{ m_time = nowAnimationTime; }

	// ���݂̃A�j���[�V�����̍ő厞�Ԃ��擾
	inline float GetNowAnimationMaxTime()const 
	{ return m_spAnimation->m_maxTime; }

private:
	std::shared_ptr<AnimationData>	m_spAnimation = nullptr; // �Đ�����A�j���[�V�����f�[�^

	bool m_isLoop = false;
	float m_time = 0.0f;
};