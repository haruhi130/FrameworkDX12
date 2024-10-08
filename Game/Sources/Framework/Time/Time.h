#pragma once

class Time : public Service
{
public:
	virtual float DeltaTime() const { return 0.0f; }
};

// 可変フレームレート
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
		// 処理が終わったあとの時間を取得
		auto now = std::chrono::system_clock::now();

		auto procTime = now - m_prevTime;

		// 1フレームにかかった時間を計算
		auto count = std::chrono::duration_cast<
			std::chrono::microseconds>(procTime).count();

		// 1秒かけたら到達する係数に変換
		m_deltaTime = count / 1000000.0f;

		// 次の計算のために現在時刻を覚える
		m_prevTime = now;
	}

	// 係数取得
	float DeltaTime()const override { return m_deltaTime; }

private:
	// 一秒間隔に変換する
	float m_deltaTime = 0.0f;

	// 前回のフレームの時間
	std::chrono::system_clock::time_point m_prevTime;
};

// 固定フレームレート
class Time_FFR : public Time
{
public:
	static const int frameRate = 60;
	float DeltaTime()const override 
	{
		return (1 / (float)frameRate) / 1000000.0f;
	}
};