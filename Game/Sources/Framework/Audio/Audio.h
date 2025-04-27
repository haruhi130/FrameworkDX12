#pragma once

class SoundEffect;
class SoundInstance;
class SoundInstance3D;

//======================================================================= =
// サウンドを管理するマネージャークラス
// ///////////////////////////////////////////////////////////////////////
// 再生管理
// サウンドアセット管理
// ///////////////////////////////////////////////////////////////////////
//========================================================================
class AudioManager
{
public:
	/// <summary>
	/// Audio初期化
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool Init();

	/// <summary>
	/// Audio更新
	/// </summary>
	void Update();

	/// <summary>
	/// 3Dサウンドのリスナー行列をセット
	/// </summary>
	/// <param name="mListenerWorld">3Dサウンドのリスナー行列</param>
	void SetListenerMatrix(const Math::Matrix& mListenerWorld);

	/// <summary>
	/// サウンド再生
	/// </summary>
	/// <param name="fileName">サウンドファイルパス</param>
	/// <param name="isLoop">ループ設定</param>
	/// <returns>成功したらサウンドインスタンスを返す</returns>
	std::shared_ptr<SoundInstance> Play(std::string_view fileName, bool isLoop = false);
	/// <param name="pos">3Dサウンドの基準座標</param>
	/// <returns>成功したらサウンドインスタンスを返す</returns>
	std::shared_ptr<SoundInstance3D> Play3D(std::string_view fileName, const Math::Vector3& pos, bool isLoop = false);

	/// <summary>
	/// プレイリストに追加登録
	/// </summary>
	/// <param name="spSound">追加するサウンド</param>
	void AddPlayList(const std::shared_ptr<SoundInstance>& spSound)
	{
		if (!spSound.get()) { return; }

		m_playList[(size_t)(spSound.get())] = spSound;
	}

	/// <summary>
	/// サウンドを全て停止
	/// </summary>
	void StopAllSound();

	/// <summary>
	/// サウンドを全て一時停止
	/// </summary>
	void PauseAllSound();

	/// <summary>
	/// サウンドを全て再開
	/// </summary>
	void ResumeAllSound();

	/// <summary>
	/// サウンドを全て停止しアセット解放
	/// </summary>
	void SoundReset();

	/// <summary>
	/// サウンドを一括読み込み
	/// </summary>
	/// <param name="fileNames">読み込む複数のサウンド名</param>
	void LoadSoundAssets(std::initializer_list<std::string_view>& fileNames);

	/// <summary>
	/// サウンドを単一読み込み
	/// </summary>
	/// <param name="fileName">読み込むサウンド名</param>
	void LoadSoundAsset(const std::string_view& fileName);

	/// <summary>
	/// サウンドを全て停止しデータクリア
	/// </summary>
	void Release();

private:
	/// <summary>
	/// サウンドデータ取得
	/// </summary>
	/// <param name="fileName">サウンドファイルパス</param>
	/// <returns>サウンドエフェクトインスタンス</returns>
	std::shared_ptr<SoundEffect> GetSound(std::string_view fileName);

	std::unique_ptr<DirectX::AudioEngine> m_upAudioEngine = nullptr;

	DirectX::AudioListener m_listener;

	std::map<size_t, std::shared_ptr<SoundInstance>> m_playList;

	std::unordered_map<std::string, std::shared_ptr<SoundEffect>> m_omSounds;

	AudioManager(){}
	~AudioManager() { Release(); }
public:
	static AudioManager& GetInstance()
	{
		static AudioManager instance;
		return instance;
	}
};

//========================================================================
// 2Dサウンド再生用インスタンス
// ///////////////////////////////////////////////////////////////////////
// 再生・停止・一時停止、再生状況の確認
// パラメータの変更を行う
// ///////////////////////////////////////////////////////////////////////
//========================================================================
class SoundInstance : public std::enable_shared_from_this<SoundInstance>
{
public:
	SoundInstance(const std::shared_ptr<SoundEffect>& spSoundEffect);

	virtual bool CreateInstance();

	/// <summary>
	/// サウンド再生
	/// </summary>
	/// <param name="isLoop">ループ設定</param>
	/// <returns>成功したらtrue</returns>
	virtual bool Play(bool isLoop = false);
	
	/// <summary>
	/// サウンド停止
	/// </summary>
	void Stop() { if (m_upInstance) { m_upInstance->Stop(); }}
	
	/// <summary>
	/// サウンド一時停止
	/// </summary>
	void Pause(){ if (m_upInstance) { m_upInstance->Pause(); }}
	
	/// <summary>
	/// サウンド再生再開
	/// </summary>
	void Resume(){ if (m_upInstance) { m_upInstance->Resume(); }}

	/// <summary>
	/// 音量設定
	/// </summary>
	/// <param name="volume">音量</param>
	void SetVolume(float volume);
	
	/// <summary>
	/// ピッチ設定
	/// </summary>
	/// <param name="pitch">ピッチ</param>
	void SetPitch(float pitch);

	/// <summary>
	/// 再生状況の取得
	/// </summary>
	/// <returns>各状況であればtrue</returns>
	bool IsPlaying();
	bool IsPause();
	bool IsStopped();
	bool IsLooped();

protected:
	std::unique_ptr<DirectX::SoundEffectInstance> m_upInstance = nullptr;

	std::shared_ptr<SoundEffect> m_spSoundData = nullptr;

	// コピー禁止
	SoundInstance(const SoundInstance& src) = delete;
	void operator=(const SoundInstance& src) = delete;
};

//========================================================================
// 3Dサウンド再生用インスタンス
// ///////////////////////////////////////////////////////////////////////
// SoundInstanceに3D座標情報を追加
// ///////////////////////////////////////////////////////////////////////
//========================================================================
class SoundInstance3D : public SoundInstance
{
public:
	SoundInstance3D(const std::shared_ptr<SoundEffect>& spSoundEffect, const DirectX::AudioListener& ownerListener);

	bool CreateInstance() override;

	bool Play(bool isLoop = false) override;

	/// <summary>
	/// サウンドを再生する座標
	/// </summary>
	/// <param name="pos">再生座標</param>
	void SetPos(const Math::Vector3& pos);

	/// <summary>
	/// 減衰倍率設定
	/// </summary>
	/// <param name="val">減衰倍率</param>
	void SetCurveDistanceScaler(float val = 1.0f);

private:
	DirectX::AudioEmitter m_emitter;

	const DirectX::AudioListener& m_ownerListener;

	// コピー禁止
	SoundInstance3D(const SoundInstance3D& src) = delete;
	void operator=(const SoundInstance3D& src) = delete;
};

//========================================================================
// Wave形式のサウンドデータを格納する
//========================================================================
class SoundEffect
{
public:
	SoundEffect(){}
	~SoundEffect(){ m_upSoundEffect = nullptr; }

	/// <summary>
	/// サウンドエフェクトインスタンス作成
	/// </summary>
	/// <param name="flag">サウンドエフェクトインスタンスフラグ</param>
	/// <returns>作成したインスタンス</returns>
	std::unique_ptr<DirectX::SoundEffectInstance> 
		CreateInstance(DirectX::SOUND_EFFECT_INSTANCE_FLAGS flag)
	{
		if (!m_upSoundEffect) { return nullptr; }
		return m_upSoundEffect->CreateInstance(flag);
	}

	/// <summary>
	/// Waveサウンド読み込み
	/// </summary>
	/// <param name="fileName">サウンドファイルパス</param>
	/// <param name="engine">使用するオーディオエンジン</param>
	/// <returns>成功したらtrue</returns>
	bool Load(std::string_view fileName, const std::unique_ptr<DirectX::AudioEngine>& engine);

private:
	std::unique_ptr<DirectX::SoundEffect> m_upSoundEffect = nullptr;

	// コピー禁止
	SoundEffect(const SoundEffect& src) = delete;
	void operator=(const SoundEffect& src) = delete;
};
