#pragma once

struct WaveData
{
	WAVEFORMATEX wavFormat = {};
	char* soundBuffer = {};
	DWORD size = {};

	~WaveData() { free(soundBuffer); }
};

class Audio
{
public:

	bool Init();

	bool LoadWaveFile(const std::wstring& wFilePath, WaveData* outData);

	bool PlayWaveSound(const std::wstring& fileName, WaveData* outData, bool isLoop = false);

	// ループ再生を終了
	bool ExitLoop();

	// 停止
	bool Stop();

	// 一時停止
	bool Pause();

	// 再開
	bool Resume();

	// 音量設定
	void SetVolume(float volume);

	// ピッチ設定
	void SetPitch(float pitch);

	// 解放
	void Release();

private:
	// 本体
	ComPtr<IXAudio2> m_cpXAudio2 = nullptr;

	// マスターボイス(出力)
	IXAudio2MasteringVoice* m_pMasteringVoice = nullptr;
	
	// ソースボイス(音の起点)
	IXAudio2SourceVoice* m_pSourceVoice = nullptr;

	Audio(){}
	~Audio() { Release(); }
public:
	static Audio& GetInstance()
	{
		static Audio instance;
		return instance;
	}
};
