#pragma once

class Time : public Service
{
public:
	/// <summary>
	/// 時間初期化
	/// </summary>
	void Start() override
	{
		m_deltaTime = 0.0f;
		m_prevTime = std::chrono::system_clock::now();
	}

	/// <summary>
	/// 時間更新
	/// </summary>
	void Update() override
	{
		auto now = std::chrono::system_clock::now();
		auto count = std::chrono::duration_cast<std::chrono::microseconds>
			(now - m_prevTime).count();

		m_deltaTime = count / 1000000.0f;
		m_prevTime = now;
	}

	/// <summary>
	/// デルタタイムを返す
	/// </summary>
	/// <returns>デルタタイム</returns>
	float DeltaTime() const { return m_deltaTime; }

private:
	float m_deltaTime = 0.0f;
	std::chrono::system_clock::time_point m_prevTime;
};
