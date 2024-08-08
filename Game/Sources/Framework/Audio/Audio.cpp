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

	// チャンク情報
	MMCKINFO chunkInfo{};

	// RIFFチャンク用
	MMCKINFO riffChunkInfo{};

	// WAVファイルを開く
	mmioHandle = mmioOpen(
		(LPWSTR)wFilePath.data(),
		nullptr,
		MMIO_READ
	);

	if (!mmioHandle)
	{
		assert(0 && "WAVファイルを開けませんでした");
		return false;
	}

	// RIFFチャンクに進入するためfccTypeにWAVEを設定
	riffChunkInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	// RIFFチャンクに進入する
	if (mmioDescend(
		mmioHandle,		// MMIOハンドル
		&riffChunkInfo,	// 取得したチャンク情報
		nullptr,		// 親チャンク
		MMIO_FINDRIFF	// 取得情報の種類s
	) != MMSYSERR_NOERROR)
	{
		assert(0 && "RIFFチャンクに進入失敗しました");
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// 侵入先のチャンクをfmt として設定
	chunkInfo.ckid = mmioFOURCC('f', 'm', 't',' ');
	if (mmioDescend(
		mmioHandle,
		&chunkInfo,
		&riffChunkInfo,
		MMIO_FINDCHUNK
	) != MMSYSERR_NOERROR)
	{
		assert(0 && "fmtチャンクが存在しません");
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// fmtデータの読み込み
	DWORD readSize = mmioRead(
		mmioHandle,					// ハンドル
		(HPSTR)&outData->wavFormat,	// 読み込み用バッファ
		chunkInfo.cksize			// バッファサイズ
	);

	if (readSize != chunkInfo.cksize)
	{
		assert(0 && "読み込みサイズが一致していません");
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// フォーマットチェック
	if (outData->wavFormat.wFormatTag != WAVE_FORMAT_PCM)
	{
		assert(0 && "Waveフォーマットエラー");
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// fmtチャンクを退出
	if (mmioAscend(
		mmioHandle,
		&chunkInfo,
		0
	) != MMSYSERR_NOERROR)
	{
		assert(0 && "fmtチャンク退出失敗");
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// dataチャンクに侵入
	chunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(
		mmioHandle,
		&chunkInfo,
		&riffChunkInfo,
		MMIO_FINDCHUNK
	) != MMSYSERR_NOERROR)
	{
		assert(0 && "dataチャンク侵入失敗");
		mmioClose(mmioHandle, MMIO_FHOPEN);
		return false;
	}

	// サイズ保存
	outData->size = chunkInfo.cksize;

	// dataチャンク読み込み
	outData->soundBuffer = new char[chunkInfo.cksize];
	readSize = mmioRead(
		mmioHandle,
		(HPSTR)outData->soundBuffer,
		chunkInfo.cksize);
	if (readSize != chunkInfo.cksize)
	{
		assert(0 && "dataチャンク読み込み失敗");
		delete[] outData->soundBuffer;
		return false;
	}

	// ファイルを閉じる
	mmioClose(mmioHandle, MMIO_FHOPEN);

	return true;
}

bool Audio::PlayWaveSound(const std::wstring& fileName, WaveData* outData, bool isLoop)
{
	if (!LoadWaveFile(fileName, outData))
	{
		assert(0 && "Waveファイル読み込み失敗");
		return false;
	}

	//===================
	// SourceVoiceの作成
	//===================

	WAVEFORMATEX waveFormat{};

	// 波形フォーマット設定
	memcpy(&waveFormat, &outData->wavFormat, sizeof(outData->wavFormat));

	// 1サンプルあたりのバッファサイズ算出
	waveFormat.wBitsPerSample = outData->wavFormat.nBlockAlign * 8 / outData->wavFormat.nChannels;

	// SourceVoice作成(フォーマットのみ)
	auto result = m_cpXAudio2->CreateSourceVoice(&m_pSourceVoice, (WAVEFORMATEX*)&waveFormat);
	if (FAILED(result))
	{
		assert(0 && "SourceVoice作成失敗");
		return false;
	}

	// 波形データをSourceVoiceに渡す
	XAUDIO2_BUFFER xAudio2Buffer = {};
	memset(&xAudio2Buffer, 0, sizeof(XAUDIO2_BUFFER));
	xAudio2Buffer.pAudioData = (BYTE*)outData->soundBuffer;
	xAudio2Buffer.Flags = XAUDIO2_END_OF_STREAM;
	xAudio2Buffer.AudioBytes = outData->size;

	// ループ設定
	xAudio2Buffer.LoopCount = isLoop ? XAUDIO2_LOOP_INFINITE : 0;

	XAUDIO2_VOICE_STATE state;
	m_pSourceVoice->GetState(&state);

	if (state.BuffersQueued == 0)
	{
		m_pSourceVoice->FlushSourceBuffers();
		m_pSourceVoice->SubmitSourceBuffer(&xAudio2Buffer);
	}

	// 実際に音を鳴らす
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
