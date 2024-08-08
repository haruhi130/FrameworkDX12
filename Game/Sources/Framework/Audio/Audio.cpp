#include "Audio.h"

bool Audio::Init()
{
	auto result = XAudio2Create(m_cpXAudio2.ReleaseAndGetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	result = m_cpXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool Audio::LoadWaveFile(const std::wstring& wFilePath, WaveData* outData)
{
	if (outData)
	{
		free(outData->soundBuffer);
	}
	else
	{
		return false;
	}

	HMMIO mmioHandle = nullptr;

	// �`�����N���
	MMCKINFO chunkInfo{};

	// RIFF�`�����N�p
	MMCKINFO riffChunkInfo{};

	// WAV�t�@�C�����J��
	mmioHandle = mmioOpen(
		(LPWSTR)wFilePath.data(),
		nullptr,
		MMIO_READ
	);

	if (!mmioHandle)
	{
		assert(0 && "WAV�t�@�C�����J���܂���ł���");
		return false;
	}

	// RIFF�`�����N�ɐi�����邽��fccType��WAVE��ݒ�
	riffChunkInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	// RIFF�`�����N�ɐi������
	if (mmioDescend(
		mmioHandle,		// MMIO�n���h��
		&riffChunkInfo,	// �擾�����`�����N���
		nullptr,		// �e�`�����N
		MMIO_FINDRIFF	// �擾���̎��s
	) != MMSYSERR_NOERROR)
	{
		assert(0 && "RIFF�`�����N�ɐi�����s���܂���");
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// �N����̃`�����N��fmt �Ƃ��Đݒ�
	chunkInfo.ckid = mmioFOURCC('f', 'm', 't',' ');
	if (mmioDescend(
		mmioHandle,
		&chunkInfo,
		&riffChunkInfo,
		MMIO_FINDCHUNK
	) != MMSYSERR_NOERROR)
	{
		assert(0 && "fmt�`�����N�����݂��܂���");
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// fmt�f�[�^�̓ǂݍ���
	DWORD readSize = mmioRead(
		mmioHandle,					// �n���h��
		(HPSTR)&outData->wavFormat,	// �ǂݍ��ݗp�o�b�t�@
		chunkInfo.cksize			// �o�b�t�@�T�C�Y
	);

	if (readSize != chunkInfo.cksize)
	{
		assert(0 && "�ǂݍ��݃T�C�Y����v���Ă��܂���");
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// �t�H�[�}�b�g�`�F�b�N
	if (outData->wavFormat.wFormatTag != WAVE_FORMAT_PCM)
	{
		assert(0 && "Wave�t�H�[�}�b�g�G���[");
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// fmt�`�����N��ޏo
	if (mmioAscend(
		mmioHandle,
		&chunkInfo,
		0
	) != MMSYSERR_NOERROR)
	{
		assert(0 && "fmt�`�����N�ޏo���s");
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// data�`�����N�ɐN��
	chunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(
		mmioHandle,
		&chunkInfo,
		&riffChunkInfo,
		MMIO_FINDCHUNK
	) != MMSYSERR_NOERROR)
	{
		assert(0 && "data�`�����N�N�����s");
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// �T�C�Y�ۑ�
	outData->size = chunkInfo.cksize;

	// data�`�����N�ǂݍ���
	outData->soundBuffer = new char[chunkInfo.cksize];
	readSize = mmioRead(
		mmioHandle,
		(HPSTR)outData->soundBuffer,
		chunkInfo.cksize);
	if (readSize != chunkInfo.cksize)
	{
		assert(0 && "data�`�����N�ǂݍ��ݎ��s");
		delete[] outData->soundBuffer;
		return false;
	}

	// �t�@�C�������
	mmioClose(mmioHandle, MMIO_FHOPEN);

	return true;
}

bool Audio::PlayWaveSound(const std::wstring& fileName, WaveData* outData, bool isLoop)
{
	if (!LoadWaveFile(fileName, outData))
	{
		assert(0 && "Wave�t�@�C���ǂݍ��ݎ��s");
		return false;
	}

	//===================
	// SourceVoice�̍쐬
	//===================

	WAVEFORMATEX waveFormat{};

	// �g�`�t�H�[�}�b�g�ݒ�
	memcpy(&waveFormat, &outData->wavFormat, sizeof(outData->wavFormat));

	// 1�T���v��������̃o�b�t�@�T�C�Y�Z�o
	waveFormat.wBitsPerSample = outData->wavFormat.nBlockAlign * 8 / outData->wavFormat.nChannels;

	// SourceVoice�쐬(�t�H�[�}�b�g�̂�)
	auto result = m_cpXAudio2->CreateSourceVoice(&m_pSourceVoice, (WAVEFORMATEX*)&waveFormat);
	if (FAILED(result))
	{
		assert(0 && "SourceVoice�쐬���s");
		return false;
	}

	// �g�`�f�[�^��SourceVoice�ɓn��
	XAUDIO2_BUFFER xAudio2Buffer = {};
	memset(&xAudio2Buffer, 0, sizeof(XAUDIO2_BUFFER));
	xAudio2Buffer.pAudioData = (BYTE*)outData->soundBuffer;
	xAudio2Buffer.Flags = XAUDIO2_END_OF_STREAM;
	xAudio2Buffer.AudioBytes = outData->size;

	// ���[�v�ݒ�
	xAudio2Buffer.LoopCount = isLoop ? XAUDIO2_LOOP_INFINITE : 0;

	XAUDIO2_VOICE_STATE state;
	m_pSourceVoice->GetState(&state);

	if (state.BuffersQueued == 0)
	{
		m_pSourceVoice->FlushSourceBuffers();
		m_pSourceVoice->SubmitSourceBuffer(&xAudio2Buffer);
	}

	// ���ۂɉ���炷
	m_pSourceVoice->Start(0);

	return true;
}

bool Audio::ExitLoop()
{
	if (m_pSourceVoice)
	{
		m_pSourceVoice->ExitLoop();
	}
	else 
	{
		return false; 
	}

	return true;
}

bool Audio::Stop()
{
	if (m_pSourceVoice)
	{
		m_pSourceVoice->Stop(0);
		m_pSourceVoice->FlushSourceBuffers();
	}

	return true;
}

bool Audio::Pause()
{
	if (m_pSourceVoice)
	{
		m_pSourceVoice->Stop(0);
	}

	return true;
}

bool Audio::Resume()
{
	if (m_pSourceVoice)
	{
		XAUDIO2_VOICE_STATE state;
		m_pSourceVoice->GetState(&state);

		if (state.BuffersQueued != 0)
		{
			m_pSourceVoice->Start();
		}
	}

	return true;
}

void Audio::SetVolume(float volume)
{
	if (m_pSourceVoice)
	{
		m_pSourceVoice->SetVolume(volume);
	}
}

void Audio::SetPitch(float pitch)
{
	if (m_pSourceVoice)
	{
		m_pSourceVoice->SetFrequencyRatio(std::max(std::min(pitch, 100.0f), XAUDIO2_MIN_FREQ_RATIO));
	}
}

void Audio::Release()
{
	Stop();
	if (m_pSourceVoice)
	{
		m_pSourceVoice->DestroyVoice();
		m_pSourceVoice = nullptr;
	}
	if (m_pMasteringVoice)
	{
		m_pMasteringVoice->DestroyVoice();
		m_pMasteringVoice = nullptr;
	}
}
