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

	// ���[�v�Đ����I��
	bool ExitLoop();

	// ��~
	bool Stop();

	// �ꎞ��~
	bool Pause();

	// �ĊJ
	bool Resume();

	// ���ʐݒ�
	void SetVolume(float volume);

	// �s�b�`�ݒ�
	void SetPitch(float pitch);

	// ���
	void Release();

private:
	// �{��
	ComPtr<IXAudio2> m_cpXAudio2 = nullptr;

	// �}�X�^�[�{�C�X(�o��)
	IXAudio2MasteringVoice* m_pMasteringVoice = nullptr;
	
	// �\�[�X�{�C�X(���̋N�_)
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
