#pragma once

class Time:public Service
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
		// �O��̎��ԂƂ̊Ԋu���v�Z
		auto procTime = now - m_prevTime;

		// �w��P�ʂŐ�����
		auto count = std::chrono::duration_cast<
			std::chrono::microseconds>(procTime).count();

		// 1�b�������瓞�B����W���ɕϊ�
		m_deltaTime = count / 1000000.0f;

		// ���̌v�Z�̂��߂Ɍ��ݎ������o����
		m_prevTime = now;
	}

	// �W���擾
	virtual float DeltaTime() { return m_deltaTime; }

private:
	// ��b�Ԋu�ɕϊ�����
	float m_deltaTime = 0.0f;

	// �O��̃t���[���̎���
	std::chrono::system_clock::time_point m_prevTime;
};

class TimeFFR : public Time
{
public:
	float DeltaTime()override { return 0.016f; }
};