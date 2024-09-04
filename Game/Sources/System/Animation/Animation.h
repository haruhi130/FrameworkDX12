#pragma once

// アニメーションキー
struct AnimeKeyQuaternion
{
	float				m_time = 0;		// 時間
	Math::Quaternion	m_quat;			// クォータニオン
};

// アニメーションキー
struct AnimeKeyVector3
{
	float			m_time = 0;			// 時間
	Math::Vector3	m_vec;				// 3Dベクトルデータ
};

// アニメーションデータ
struct AnimationData
{
	std::string		m_name;				// アニメーション名
	float			m_maxTime = 0;		// アニメーション最大時間

	struct Channel
	{
		int	m_nodeOffset = -1;	// 対象モデルノードのOffset値

		// 各チャンネル
		std::vector<AnimeKeyVector3>	m_translations;	// 位置キーリスト
		std::vector<AnimeKeyQuaternion>	m_rotations;	// 回転キーリスト
		std::vector<AnimeKeyVector3>	m_scales;		// 拡縮キーリスト

		void Interpolate(Math::Matrix& rDst, float time);
		bool InterpolateTranslations(Math::Vector3& result, float time);
		bool InterpolateRotations(Math::Quaternion& result, float time);
		bool InterpolateScales(Math::Vector3& result, float time);
	};

	// 全ノード用アニメーションデータ
	std::vector<Channel>	m_channels;
};

class Animator
{
public:
	// アニメーションデータの取得
	const std::shared_ptr<AnimationData> GetAnimationData()const { return m_spAnimation; }

	// アニメーションのセット
	inline void SetAnimation(const std::shared_ptr<AnimationData>& rData, const bool isLoop = true)
	{
		m_spAnimation = rData;
		m_isLoop = isLoop;

		m_time = 0.0f;
	}

	// アニメーションが終了しているか
	inline bool IsAnimationEnd() const
	{
		if (m_spAnimation == nullptr) { return true; }
		if (m_time >= m_spAnimation->m_maxTime) { return true; }

		return false;
	}

	// アニメーションの更新
	void ProgressTime(std::vector<ModelWork::Node>& rNodes, float speed = 1.0f);

	// アニメーションのリセット
	inline void ResetAdvanceTime() 
	{ m_time = 0; }

	// 現在のアニメーション時間を取得
	inline float GetNowAnimationTime()const
	{ return m_time; }

	// 現在のアニメーション時間をセット
	inline void SetNowAnimationTime(float nowAnimationTime)
	{ m_time = nowAnimationTime; }

	// 現在のアニメーションの最大時間を取得
	inline float GetNowAnimationMaxTime()const 
	{ return m_spAnimation->m_maxTime; }

private:
	std::shared_ptr<AnimationData>	m_spAnimation = nullptr; // 再生するアニメーションデータ

	bool m_isLoop = false;
	float m_time = 0.0f;
};