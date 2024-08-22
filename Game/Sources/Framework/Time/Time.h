#pragma once

class Time:public Service
{
public:
	void Start()override
	{
		m_prevTime = std::chrono::system_clock::now();
		m_deltaTime = 0.0f;
	}

	void Update()override
	{
		// 処理が終わったあとの時間を取得
		auto now = std::chrono::system_clock::now();
		// 前回の時間との間隔を計算
		auto procTime = now - m_prevTime;

		// 指定単位で数える
		auto count = std::chrono::duration_cast<
			std::chrono::microseconds>(procTime).count();

		// 1秒かけたら到達する係数に変換
		m_deltaTime = count / 1000000.0f;

		// 次の計算のために現在時刻を覚える
		m_prevTime = now;
	}

private:
	// 一秒間隔に変換する
	float m_deltaTime = 0.0f;

	// 前回のフレームの時間
	std::chrono::system_clock::time_point m_prevTime;
};

class TimeFFR : public Time
{
public:
	float DeltaTime() { return 0.016f; }
};