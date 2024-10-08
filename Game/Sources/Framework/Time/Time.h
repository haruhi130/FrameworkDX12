#pragma once

class Time : public Service
{
public:
	virtual float DeltaTime() const { return 0.0f; }
};

// �σt���[�����[�g
class Time_VRR : public Time
{
public:
	void Start()override
	{
		m_deltaTime = 0.0f;
		m_prevTime = std::chrono::system_clock::now();
	}

	void Update()override
	{
		// �������I��������Ƃ̎��Ԃ��擾
		auto now = std::chrono::system_clock::now();

		auto procTime = now - m_prevTime;

		// 1�t���[���ɂ����������Ԃ��v�Z
		auto count = std::chrono::duration_cast<
			std::chrono::microseconds>(procTime).count();

		// 1�b�������瓞�B����W���ɕϊ�
		m_deltaTime = count / 1000000.0f;

		// ���̌v�Z�̂��߂Ɍ��ݎ������o����
		m_prevTime = now;
	}

	// �W���擾
	float DeltaTime()const override { return m_deltaTime; }

private:
	// ��b�Ԋu�ɕϊ�����
	float m_deltaTime = 0.0f;

	// �O��̃t���[���̎���
	std::chrono::system_clock::time_point m_prevTime;
};

// �Œ�t���[�����[�g
class Time_FFR : public Time
{
public:
	static const int frameRate = 60;
	float DeltaTime()const override 
	{
		return (1 / (float)frameRate) / 1000000.0f;
	}
};