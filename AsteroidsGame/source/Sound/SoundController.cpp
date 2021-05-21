#include "SoundController.h"

HRESULT SoundController::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;
}

HRESULT SoundController::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

HRESULT SoundController::SetupSourceVoice(const char* fileLocation, IXAudio2SourceVoice*& pSourceVoice, XAUDIO2_BUFFER& buffer)
{
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	// Open the file
	HANDLE hFile = CreateFile(
		fileLocation,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
	if (filetype != fourccWAVE)
		return S_FALSE;
	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE* pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
	buffer.pAudioData = pDataBuffer;  //buffer containing audio data
	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer				
	if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx))) return hr;
}

HRESULT SoundController::playAudio(IXAudio2SourceVoice*& pSourceVoice, XAUDIO2_BUFFER& buffer, bool loopAudio, float volume)
{
	if (loopAudio) {
		buffer.LoopBegin = 0;
		buffer.LoopCount = XAUDIO2_MAX_LOOP_COUNT;
	}
	if (FAILED(hr = pSourceVoice->SetVolume(volume)))
		return hr;
	if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
		return hr;
	if (FAILED(hr = pSourceVoice->Start(0)))
		return hr;
}

SoundController::SoundController()
{
}

SoundController::~SoundController()
{
	this->stopAllSoundLoops();
}

HRESULT SoundController::Initialize()
{
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
		return hr;

	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
		return hr;

	IXAudio2MasteringVoice* pMasterVoice = nullptr;
	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
		return hr;

	SetupSourceVoice(backgroundFile, pBackgroundVoice, backgroundBuffer);
	SetupSourceVoice(destructionFile, pSaucerDestructionVoice, saucerDestructionBuffer);
	SetupSourceVoice(spaceShipDestructionFile, pSpaceShipDestructionVoice, spaceShipDestructionBuffer);
	SetupSourceVoice(shootingFile, pShootingVoice, shootingBuffer);
	SetupSourceVoice(saucerFile, pSaucerVoice, saucerBuffer);
	SetupSourceVoice(smallSaucerFile, pSmallSaucerVoice, smallSaucerBuffer);
	SetupSourceVoice(thrustFile, pThrustVoice, thrustBuffer);
}

HRESULT SoundController::playBackgroundSound()
{
	return playAudio(pBackgroundVoice, backgroundBuffer, true, 0.5f);
}

HRESULT SoundController::pitchBackgroundSound(float pitch)
{
	if (FAILED(hr = pBackgroundVoice->SetFrequencyRatio(pitch)))
		return hr;
}

HRESULT SoundController::playAsteroidDestructionSound()
{
	SetupSourceVoice(destructionFile, pDestructionVoice, destructionBuffer);
	return playAudio(pDestructionVoice, destructionBuffer, false, 0.3f);
}

HRESULT SoundController::playSaucerDestructionSound()
{
	
	return playAudio(pSaucerDestructionVoice, destructionBuffer, false, 0.8f);
}

HRESULT SoundController::playspaceShipDestructionSound()
{
	return playAudio(pSpaceShipDestructionVoice, spaceShipDestructionBuffer, false, 0.4f);
}

HRESULT SoundController::playSaucerSound()
{
	return playAudio(pSaucerVoice, saucerBuffer, true);
}

HRESULT SoundController::stopSaucerSound()
{
	if (pSaucerVoice != nullptr && FAILED(hr = pSaucerVoice->Stop(0)))
		return hr;
	if (pSaucerVoice != nullptr && FAILED(hr = pSmallSaucerVoice->Stop(0)))
		return hr;
}

HRESULT SoundController::playSmallSaucerSound()
{
	return playAudio(pSmallSaucerVoice, smallSaucerBuffer, true);
}

HRESULT SoundController::playShootingSound()
{
	return playAudio(pShootingVoice, shootingBuffer, false, 0.5f);
}

HRESULT SoundController::playThrustSound()
{
	if (!this->thrusting) {
		this->thrusting = true;
		return playAudio(pThrustVoice, thrustBuffer, true);
	}
}

HRESULT SoundController::stopThrustSound()
{
	if (this->thrusting) {
		this->thrusting = false;
		if (FAILED(hr = pThrustVoice->Stop(0)))
			return hr;
	}
}

HRESULT SoundController::stopAllSoundLoops()
{
	if (pBackgroundVoice != nullptr && FAILED(hr = pBackgroundVoice->Stop(0)))
		return hr;
	if (pSaucerVoice != nullptr && FAILED(hr = pSaucerVoice->Stop(0)))
		return hr;
	if (pSmallSaucerVoice != nullptr && FAILED(hr = pSmallSaucerVoice->Stop(0)))
		return hr;
	if (pThrustVoice != nullptr && FAILED(hr = pThrustVoice->Stop(0)))
		return hr;
}
