#pragma once

class SoundEffect;
class SoundInstance;
class SoundInstance3D;

//======================================================================= =
// �T�E���h���Ǘ�����}�l�[�W���[�N���X
// ///////////////////////////////////////////////////////////////////////
// �Đ��Ǘ�
// �T�E���h�A�Z�b�g�Ǘ�
// ///////////////////////////////////////////////////////////////////////
//========================================================================
class AudioManager
{
public:
	bool Init();

	void Update();

	void SetListenerMatrix(const Math::Matrix& mWorld);

	// �T�E���h�Đ�
	std::shared_ptr<SoundInstance> Play(std::string_view fileName, bool isLoop = false);
	std::shared_ptr<SoundInstance3D> Play3D(std::string_view fileName, const Math::Vector3& pos, bool isLoop = false);

	void AddPlayList(const std::shared_ptr<SoundInstance>& spSound)
	{
		if (!spSound.get()) { return; }

		m_playList[(size_t)(spSound.get())] = spSound;
	}

	// �T�E���h���X�g�̉���S�Ē�~
	void StopAllSound();
	// �T�E���h���X�g�̉���S�Ĉꎞ��~
	void PauseAllSound();
	// �T�E���h���X�g�̉���S�čĊJ
	void ResumeAllSound();

	// �T�E���h���X�g�̉���S�Ē�~���A�Z�b�g�����
	void SoundReset();

	// �T�E���h�A�Z�b�g�̈ꊇ�ǂݍ���
	void LoadSoundAssets(std::initializer_list<std::string_view>& fileNames);

	// ���
	void Release();

private:
	// �T�E���h�f�[�^�̎擾/���[�h
	std::shared_ptr<SoundEffect> GetSound(std::string_view fileName);

	// DirectXAudioEngine
	std::unique_ptr<DirectX::AudioEngine> m_upAudioEngine = nullptr;

	// 3D�T�E���h�p���X�i�[
	DirectX::AudioListener m_listener;

	// �Đ����̃T�E���h���X�g
	std::map<size_t, std::shared_ptr<SoundInstance>> m_playList;

	// �T�E���h�A�Z�b�g���X�g
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
// 2D�T�E���h�Đ��p�C���X�^���X
// ///////////////////////////////////////////////////////////////////////
// �Đ��E��~�E�ꎞ��~�A�Đ��󋵂̊m�F
// �p�����[�^�̕ύX���s��
// ///////////////////////////////////////////////////////////////////////
//========================================================================
class SoundInstance : public std::enable_shared_from_this<SoundInstance>
{
public:
	SoundInstance(const std::shared_ptr<SoundEffect>& spSoundEffect);

	virtual bool CreateInstance();

	// �Đ�
	virtual bool Play(bool isLoop = false);
	
	// ��~
	void Stop() { if (m_upInstance) { m_upInstance->Stop(); }}
	// �ꎞ��~
	void Pause(){ if (m_upInstance) { m_upInstance->Pause(); }}
	// �ĊJ
	void Resume(){ if (m_upInstance) { m_upInstance->Resume(); }}

	// �{�����[���ݒ�
	void SetVolume(float volume);
	
	// �s�b�`�ݒ�
	void SetPitch(float pitch);

	// �Đ��󋵂̎擾
	bool IsPlaying();
	bool IsPause();
	bool IsStopped();
	bool IsLooped();

protected:
	// �T�E���h�G�t�F�N�g�C���X�^���X
	std::unique_ptr<DirectX::SoundEffectInstance> m_upInstance = nullptr;

	// �Đ�����T�E���h�̌��f�[�^
	std::shared_ptr<SoundEffect> m_spSoundData = nullptr;

	// �R�s�[�֎~
	SoundInstance(const SoundInstance& src) = delete;
	void operator=(const SoundInstance& src) = delete;
};

//========================================================================
// 3D�T�E���h�Đ��p�C���X�^���X
// ///////////////////////////////////////////////////////////////////////
// SoundInstance��3D���W����ǉ�
// ///////////////////////////////////////////////////////////////////////
//========================================================================
class SoundInstance3D : public SoundInstance
{
public:
	SoundInstance3D(const std::shared_ptr<SoundEffect>& spSoundEffect, const DirectX::AudioListener& ownerListener);

	bool CreateInstance() override;

	bool Play(bool isLoop = false) override;

	void SetPos(const Math::Vector3& pos);

	// �����{���ݒ�
	void SetCurveDistanceScaler(float val = 1.0f);

private:
	// �G�~�b�^�[
	DirectX::AudioEmitter m_emitter;

	// 3D�T�E���h�p���X�i�[
	const DirectX::AudioListener& m_ownerListener;

	// �R�s�[�֎~
	SoundInstance3D(const SoundInstance3D& src) = delete;
	void operator=(const SoundInstance3D& src) = delete;
};

//========================================================================
// Wave�`���̃T�E���h�f�[�^���i�[����
//========================================================================
class SoundEffect
{
public:
	SoundEffect(){}
	~SoundEffect(){ m_upSoundEffect = nullptr; }

	// �T�E���h�G�t�F�N�g�C���X�^���X�쐬
	std::unique_ptr<DirectX::SoundEffectInstance> 
		CreateInstance(DirectX::SOUND_EFFECT_INSTANCE_FLAGS flag)
	{
		if (!m_upSoundEffect) { return nullptr; }
		return m_upSoundEffect->CreateInstance(flag);
	}

	// Wave�T�E���h�ǂݍ���
	bool Load(std::string_view fileName, const std::unique_ptr<DirectX::AudioEngine>& engine);

private:
	// �T�E���h�G�t�F�N�g
	std::unique_ptr<DirectX::SoundEffect> m_upSoundEffect = nullptr;

	// �R�s�[�֎~
	SoundEffect(const SoundEffect& src) = delete;
	void operator=(const SoundEffect& src) = delete;
};
