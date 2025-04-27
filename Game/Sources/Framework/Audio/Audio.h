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
	/// <summary>
	/// Audio������
	/// </summary>
	/// <returns>����������true</returns>
	bool Init();

	/// <summary>
	/// Audio�X�V
	/// </summary>
	void Update();

	/// <summary>
	/// 3D�T�E���h�̃��X�i�[�s����Z�b�g
	/// </summary>
	/// <param name="mListenerWorld">3D�T�E���h�̃��X�i�[�s��</param>
	void SetListenerMatrix(const Math::Matrix& mListenerWorld);

	/// <summary>
	/// �T�E���h�Đ�
	/// </summary>
	/// <param name="fileName">�T�E���h�t�@�C���p�X</param>
	/// <param name="isLoop">���[�v�ݒ�</param>
	/// <returns>����������T�E���h�C���X�^���X��Ԃ�</returns>
	std::shared_ptr<SoundInstance> Play(std::string_view fileName, bool isLoop = false);
	/// <param name="pos">3D�T�E���h�̊���W</param>
	/// <returns>����������T�E���h�C���X�^���X��Ԃ�</returns>
	std::shared_ptr<SoundInstance3D> Play3D(std::string_view fileName, const Math::Vector3& pos, bool isLoop = false);

	/// <summary>
	/// �v���C���X�g�ɒǉ��o�^
	/// </summary>
	/// <param name="spSound">�ǉ�����T�E���h</param>
	void AddPlayList(const std::shared_ptr<SoundInstance>& spSound)
	{
		if (!spSound.get()) { return; }

		m_playList[(size_t)(spSound.get())] = spSound;
	}

	/// <summary>
	/// �T�E���h��S�Ē�~
	/// </summary>
	void StopAllSound();

	/// <summary>
	/// �T�E���h��S�Ĉꎞ��~
	/// </summary>
	void PauseAllSound();

	/// <summary>
	/// �T�E���h��S�čĊJ
	/// </summary>
	void ResumeAllSound();

	/// <summary>
	/// �T�E���h��S�Ē�~���A�Z�b�g���
	/// </summary>
	void SoundReset();

	/// <summary>
	/// �T�E���h���ꊇ�ǂݍ���
	/// </summary>
	/// <param name="fileNames">�ǂݍ��ޕ����̃T�E���h��</param>
	void LoadSoundAssets(std::initializer_list<std::string_view>& fileNames);

	/// <summary>
	/// �T�E���h��P��ǂݍ���
	/// </summary>
	/// <param name="fileName">�ǂݍ��ރT�E���h��</param>
	void LoadSoundAsset(const std::string_view& fileName);

	/// <summary>
	/// �T�E���h��S�Ē�~���f�[�^�N���A
	/// </summary>
	void Release();

private:
	/// <summary>
	/// �T�E���h�f�[�^�擾
	/// </summary>
	/// <param name="fileName">�T�E���h�t�@�C���p�X</param>
	/// <returns>�T�E���h�G�t�F�N�g�C���X�^���X</returns>
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

	/// <summary>
	/// �T�E���h�Đ�
	/// </summary>
	/// <param name="isLoop">���[�v�ݒ�</param>
	/// <returns>����������true</returns>
	virtual bool Play(bool isLoop = false);
	
	/// <summary>
	/// �T�E���h��~
	/// </summary>
	void Stop() { if (m_upInstance) { m_upInstance->Stop(); }}
	
	/// <summary>
	/// �T�E���h�ꎞ��~
	/// </summary>
	void Pause(){ if (m_upInstance) { m_upInstance->Pause(); }}
	
	/// <summary>
	/// �T�E���h�Đ��ĊJ
	/// </summary>
	void Resume(){ if (m_upInstance) { m_upInstance->Resume(); }}

	/// <summary>
	/// ���ʐݒ�
	/// </summary>
	/// <param name="volume">����</param>
	void SetVolume(float volume);
	
	/// <summary>
	/// �s�b�`�ݒ�
	/// </summary>
	/// <param name="pitch">�s�b�`</param>
	void SetPitch(float pitch);

	/// <summary>
	/// �Đ��󋵂̎擾
	/// </summary>
	/// <returns>�e�󋵂ł����true</returns>
	bool IsPlaying();
	bool IsPause();
	bool IsStopped();
	bool IsLooped();

protected:
	std::unique_ptr<DirectX::SoundEffectInstance> m_upInstance = nullptr;

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

	/// <summary>
	/// �T�E���h���Đ�������W
	/// </summary>
	/// <param name="pos">�Đ����W</param>
	void SetPos(const Math::Vector3& pos);

	/// <summary>
	/// �����{���ݒ�
	/// </summary>
	/// <param name="val">�����{��</param>
	void SetCurveDistanceScaler(float val = 1.0f);

private:
	DirectX::AudioEmitter m_emitter;

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

	/// <summary>
	/// �T�E���h�G�t�F�N�g�C���X�^���X�쐬
	/// </summary>
	/// <param name="flag">�T�E���h�G�t�F�N�g�C���X�^���X�t���O</param>
	/// <returns>�쐬�����C���X�^���X</returns>
	std::unique_ptr<DirectX::SoundEffectInstance> 
		CreateInstance(DirectX::SOUND_EFFECT_INSTANCE_FLAGS flag)
	{
		if (!m_upSoundEffect) { return nullptr; }
		return m_upSoundEffect->CreateInstance(flag);
	}

	/// <summary>
	/// Wave�T�E���h�ǂݍ���
	/// </summary>
	/// <param name="fileName">�T�E���h�t�@�C���p�X</param>
	/// <param name="engine">�g�p����I�[�f�B�I�G���W��</param>
	/// <returns>����������true</returns>
	bool Load(std::string_view fileName, const std::unique_ptr<DirectX::AudioEngine>& engine);

private:
	std::unique_ptr<DirectX::SoundEffect> m_upSoundEffect = nullptr;

	// �R�s�[�֎~
	SoundEffect(const SoundEffect& src) = delete;
	void operator=(const SoundEffect& src) = delete;
};
