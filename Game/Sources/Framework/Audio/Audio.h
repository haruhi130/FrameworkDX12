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
	bool Init();

	void Update();

	void SetListenerMatrix(const Math::Matrix& mWorld);

	// サウンド再生
	std::shared_ptr<SoundInstance> Play(std::string_view fileName, bool isLoop = false);
	std::shared_ptr<SoundInstance3D> Play3D(std::string_view fileName, const Math::Vector3& pos, bool isLoop = false);

	void AddPlayList(const std::shared_ptr<SoundInstance>& spSound)
	{
		if (!spSound.get()) { return; }

		m_playList[(size_t)(spSound.get())] = spSound;
	}

	// サウンドリストの音を全て停止
	void StopAllSound();
	// サウンドリストの音を全て一時停止
	void PauseAllSound();
	// サウンドリストの音を全て再開
	void ResumeAllSound();

	// サウンドリストの音を全て停止しアセットを解放
	void SoundReset();

	// サウンドアセットの一括読み込み
	void LoadSoundAssets(std::initializer_list<std::string_view>& fileNames);

	// 解放
	void Release();

private:
	// サウンドデータの取得/ロード
	std::shared_ptr<SoundEffect> GetSound(std::string_view fileName);

	// DirectXAudioEngine
	std::unique_ptr<DirectX::AudioEngine> m_upAudioEngine = nullptr;

	// 3Dサウンド用リスナー
	DirectX::AudioListener m_listener;

	// 再生中のサウンドリスト
	std::map<size_t, std::shared_ptr<SoundInstance>> m_playList;

	// サウンドアセットリスト
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

	// 再生
	virtual bool Play(bool isLoop = false);
	
	// 停止
	void Stop() { if (m_upInstance) { m_upInstance->Stop(); }}
	// 一時停止
	void Pause(){ if (m_upInstance) { m_upInstance->Pause(); }}
	// 再開
	void Resume(){ if (m_upInstance) { m_upInstance->Resume(); }}

	// ボリューム設定
	void SetVolume(float volume);
	
	// ピッチ設定
	void SetPitch(float pitch);

	// 再生状況の取得
	bool IsPlaying();
	bool IsPause();
	bool IsStopped();
	bool IsLooped();

protected:
	// サウンドエフェクトインスタンス
	std::unique_ptr<DirectX::SoundEffectInstance> m_upInstance = nullptr;

	// 再生するサウンドの元データ
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

	void SetPos(const Math::Vector3& pos);

	// 減衰倍率設定
	void SetCurveDistanceScaler(float val = 1.0f);

private:
	// エミッター
	DirectX::AudioEmitter m_emitter;

	// 3Dサウンド用リスナー
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

	// サウンドエフェクトインスタンス作成
	std::unique_ptr<DirectX::SoundEffectInstance> 
		CreateInstance(DirectX::SOUND_EFFECT_INSTANCE_FLAGS flag)
	{
		if (!m_upSoundEffect) { return nullptr; }
		return m_upSoundEffect->CreateInstance(flag);
	}

	// Waveサウンド読み込み
	bool Load(std::string_view fileName, const std::unique_ptr<DirectX::AudioEngine>& engine);

private:
	// サウンドエフェクト
	std::unique_ptr<DirectX::SoundEffect> m_upSoundEffect = nullptr;

	// コピー禁止
	SoundEffect(const SoundEffect& src) = delete;
	void operator=(const SoundEffect& src) = delete;
};
