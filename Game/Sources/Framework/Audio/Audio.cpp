#include "Audio.h"

// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// 
// AudioManager
// 
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
bool AudioManager::Init()
{
	// AudioEngine初期化
	DirectX::AUDIO_ENGINE_FLAGS flags = DirectX::AudioEngine_ReverbUseFilters;

	if (!m_upAudioEngine)
	{
		m_upAudioEngine = std::make_unique<DirectX::AudioEngine>(flags);
		m_upAudioEngine->SetReverb(DirectX::Reverb_Default);
	}

	m_listener.OrientFront = { 0,0,1 };

	return true;
}

void AudioManager::Update()
{
	if (m_upAudioEngine != nullptr)
	{
		m_upAudioEngine->Update();
	}

	// ストップさせたインスタンスは終了と判定しリストから削除
	for (auto it = m_playList.begin(); it != m_playList.end();)
	{
		if (it->second->IsStopped())
		{
			it = m_playList.erase(it);

			continue;
		}
		++it;
	}
}

void AudioManager::SetListenerMatrix(const Math::Matrix& mWorld)
{
	m_listener.SetPosition(mWorld.Translation());

	m_listener.OrientFront = mWorld.Backward();
}

std::shared_ptr<SoundInstance> AudioManager::Play
(std::string_view fileName, bool isLoop)
{
	if (!m_upAudioEngine) { return nullptr; }

	std::shared_ptr<SoundEffect> spSoundData = GetSound(fileName);

	if (!spSoundData) { return nullptr; }

	std::shared_ptr<SoundInstance> spInstance = std::make_shared<SoundInstance>(spSoundData);

	if (!spInstance->CreateInstance()) { return nullptr; }

	if (!spInstance->Play(isLoop)) { return nullptr; }

	AddPlayList(spInstance);

	return spInstance;
}

std::shared_ptr<SoundInstance3D> AudioManager::Play3D
(std::string_view fileName, const Math::Vector3& pos, bool isLoop)
{
	if (!m_upAudioEngine) { return nullptr; }

	std::shared_ptr<SoundEffect> spSoundData = GetSound(fileName);

	if (!spSoundData) { return nullptr; }

	std::shared_ptr<SoundInstance3D> spInstance = std::make_shared<SoundInstance3D>(spSoundData, m_listener);

	if (!spInstance->CreateInstance()) { return nullptr; }

	if (!spInstance->Play(isLoop)) { return nullptr; }

	spInstance->SetPos(pos);

	AddPlayList(spInstance);

	return spInstance;
}

void AudioManager::StopAllSound()
{
	auto it = m_playList.begin();
	while (it != m_playList.end())
	{
		(*it).second->Stop();
		++it;
	}
}

void AudioManager::PauseAllSound()
{
	auto it = m_playList.begin();
	while (it != m_playList.end())
	{
		(*it).second->Pause();
		++it;
	}
}

void AudioManager::ResumeAllSound()
{
	auto it = m_playList.begin();
	while (it != m_playList.end())
	{
		(*it).second->Resume();
		++it;
	}
}

void AudioManager::SoundReset()
{
	StopAllSound();

	m_omSounds.clear();
}

void AudioManager::LoadSoundAssets(std::initializer_list<std::string_view>& fileNames)
{
	for (std::string_view fileName : fileNames)
	{
		auto itFound = m_omSounds.find(fileName.data());

		if (itFound != m_omSounds.end()) { continue; }

		auto newSound = std::make_shared<SoundEffect>();
		if (!newSound->Load(fileName, m_upAudioEngine))
		{
			assert(0 && "サウンドアセット読み込み失敗");
			continue;
		}
		m_omSounds.emplace(fileName, newSound);
	}
}

void AudioManager::LoadSoundAsset(const std::string_view& fileName)
{
	auto itFound = m_omSounds.find(fileName.data());

	if (itFound != m_omSounds.end()) { return; }

	auto newSound = std::make_shared<SoundEffect>();
	if (!newSound->Load(fileName, m_upAudioEngine))
	{
		assert(0 && "サウンドアセット読み込み失敗");
	}
	m_omSounds.emplace(fileName, newSound);
}

void AudioManager::Release()
{
	StopAllSound();

	m_playList.clear();
	m_omSounds.clear();
	m_upAudioEngine = nullptr;
}

std::shared_ptr<SoundEffect> AudioManager::GetSound(std::string_view fileName)
{
	auto itFound = m_omSounds.find(fileName.data());

	// 登録チェック
	if (itFound != m_omSounds.end())
	{
		return (*itFound).second;
	}
	else
	{
		// 生成
		auto newSound = std::make_shared<SoundEffect>();
		if (!newSound->Load(fileName, m_upAudioEngine))
		{
			// 読み込み失敗時はnull
			return nullptr;
		}

		// 登録
		m_omSounds.emplace(fileName, newSound);

		return newSound;
	}
}

// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// 
// SoundInstance
// 
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
SoundInstance::SoundInstance(const std::shared_ptr<SoundEffect>& spSoundEffect)
	:m_spSoundData(spSoundEffect) {}

bool SoundInstance::CreateInstance()
{
	if (!m_spSoundData) { return false; }

	DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags = DirectX::SoundEffectInstance_Default;

	m_upInstance = (m_spSoundData->CreateInstance(flags));

	return true;
}

bool SoundInstance::Play(bool isLoop)
{
	if (!m_upInstance) { return false; }

	Stop();

	m_upInstance->Play(isLoop);

	return true;
}

void SoundInstance::SetVolume(float volume)
{
	if (!m_upInstance) { return; }

	m_upInstance->SetVolume(volume);
}

void SoundInstance::SetPitch(float pitch)
{
	if (!m_upInstance) { return; }

	m_upInstance->SetPitch(pitch);
}

bool SoundInstance::IsPlaying()
{
	if (!m_upInstance) { return false; }

	return (m_upInstance->GetState() == DirectX::SoundState::PLAYING);
}

bool SoundInstance::IsPause()
{
	if (!m_upInstance) { return false; }

	return (m_upInstance->GetState() == DirectX::SoundState::PAUSED);
}

bool SoundInstance::IsStopped()
{
	if (!m_upInstance) { return false; }

	return (m_upInstance->GetState() == DirectX::SoundState::STOPPED);
}

bool SoundInstance::IsLooped()
{
	if (!m_upInstance) { return false; }

	return m_upInstance->IsLooped();
}

// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// 
// SoundInstance3D
// 
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
SoundInstance3D::SoundInstance3D(const std::shared_ptr<SoundEffect>& spSoundEffect, const DirectX::AudioListener& ownerListener)
	:SoundInstance(spSoundEffect), m_ownerListener(ownerListener) {}

bool SoundInstance3D::CreateInstance()
{
	if (!m_spSoundData) { return false; }

	DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags = DirectX::SoundEffectInstance_Default |
		DirectX::SoundEffectInstance_Use3D | DirectX::SoundEffectInstance_ReverbUseFilters;

	m_upInstance = (m_spSoundData->CreateInstance(flags));

	return true;
}

bool SoundInstance3D::Play(bool isLoop)
{
	if (!m_upInstance) { return false; }

	SoundInstance::Play(isLoop);

	return true;
}

void SoundInstance3D::SetPos(const Math::Vector3& pos)
{
	if (!m_upInstance) { return; }

	m_emitter.SetPosition(pos);

	m_upInstance->Apply3D(m_ownerListener, m_emitter, false);
}

void SoundInstance3D::SetCurveDistanceScaler(float val)
{
	if (!m_upInstance) { return; }

	m_emitter.CurveDistanceScaler = val;

	m_upInstance->Apply3D(m_ownerListener, m_emitter, false);
}

// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// 
// SoundEffect
// 
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
bool SoundEffect::Load(std::string_view fileName, const std::unique_ptr<DirectX::AudioEngine>& engine)
{
	if (engine.get() != nullptr)
	{
		try
		{
			// wstringに変換
			std::wstring wFileName = sjis_to_wide(fileName.data());

			// 音データ読み込み
			m_upSoundEffect = std::make_unique<DirectX::SoundEffect>(engine.get(), wFileName.c_str());
		}
		catch (...)
		{
			assert(0 && "Sound File Load Error");

			return false;
		}
	}

	return true;
}
