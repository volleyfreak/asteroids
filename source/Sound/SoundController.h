#pragma once
#include <xaudio2.h>
#include <string>

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

class SoundController
{
private: 
	HRESULT hr;
	IXAudio2* pXAudio2 = nullptr;

	bool thrusting = false;

	IXAudio2SourceVoice* pBackgroundVoice;
	const char* backgroundFile = __TEXT("./res/sounds/Background.wav");
	XAUDIO2_BUFFER backgroundBuffer = { 0 };

	IXAudio2SourceVoice* pSpaceCraftDestructionVoice;
	const char* spaceCraftDestructionFile = __TEXT("./res/sounds/SpacecraftDestruction.wav");
	XAUDIO2_BUFFER spaceCraftDestructionBuffer = { 0 };

	IXAudio2SourceVoice* pDestructionVoice;
	const char* destructionFile = __TEXT("./res/sounds/Destruction.wav");
	XAUDIO2_BUFFER destructionBuffer = { 0 };

	IXAudio2SourceVoice* pShootingVoice;
	const char* shootingFile = __TEXT("./res/sounds/Shooting.wav");
	XAUDIO2_BUFFER shootingBuffer = { 0 };

	IXAudio2SourceVoice* pSaucerVoice;
	const char* saucerFile = __TEXT("./res/sounds/SaucerBackground_loop.wav");
	XAUDIO2_BUFFER saucerBuffer = { 0 };
	
	IXAudio2SourceVoice* pSmallSaucerVoice;
	const char* smallSaucerFile = __TEXT("./res/sounds/small_saucer_loop.wav");
	XAUDIO2_BUFFER smallSaucerBuffer = { 0 };
	
	IXAudio2SourceVoice* pThrustVoice;
	const char* thrustFile = __TEXT("./res/sounds/Thrust_loop.wav");
	XAUDIO2_BUFFER thrustBuffer = { 0 };


	
	int noBackgroundSound = -1;

	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
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

	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
	{
		HRESULT hr = S_OK;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
			return HRESULT_FROM_WIN32(GetLastError());
		DWORD dwRead;
		if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

public:
	HRESULT Initialize() {		
		hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(hr))
			return hr;
				
		if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
			return hr;

		IXAudio2MasteringVoice* pMasterVoice = nullptr;
		if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
			return hr;

		SetupSourceVoice(backgroundFile, pBackgroundVoice, backgroundBuffer);
		SetupSourceVoice(spaceCraftDestructionFile, pSpaceCraftDestructionVoice, spaceCraftDestructionBuffer);		
		SetupSourceVoice(shootingFile, pShootingVoice, shootingBuffer);
		SetupSourceVoice(saucerFile, pSaucerVoice, saucerBuffer);
		SetupSourceVoice(smallSaucerFile, pSmallSaucerVoice, smallSaucerBuffer);
		SetupSourceVoice(thrustFile, pThrustVoice, thrustBuffer);
	}
	HRESULT SetupSourceVoice(const char* fileLocation, IXAudio2SourceVoice*& pSourceVoice, XAUDIO2_BUFFER& buffer) {
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
	HRESULT playAudio(IXAudio2SourceVoice*& pSourceVoice, XAUDIO2_BUFFER& buffer, bool loopAudio = false, float volume = 1.0f) {
		if (loopAudio) {
			buffer.LoopBegin = 0;
			//buffer.LoopLength = 100000;
			buffer.LoopCount = XAUDIO2_MAX_LOOP_COUNT;
		}
		if (FAILED(hr = pSourceVoice->SetVolume(volume)))
			return hr;
		if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
			return hr;
		if (FAILED(hr = pSourceVoice->Start(0)))
			return hr;
	}

	HRESULT playBackgroundSound() {
		return playAudio(pBackgroundVoice, backgroundBuffer, true, 0.5f);
	}
	HRESULT pitchBackgroundSound(float pitch) {
		if (FAILED(hr = pBackgroundVoice->SetFrequencyRatio(pitch)))
			return hr;
	}

	HRESULT playAsteroidDestructionSound() {
		SetupSourceVoice(destructionFile, pDestructionVoice, destructionBuffer);
		return playAudio(pDestructionVoice, destructionBuffer, false, 0.3f);
	}

	HRESULT playSaucerDestructionSound() {
		SetupSourceVoice(destructionFile, pDestructionVoice, destructionBuffer);
		return playAudio(pDestructionVoice, destructionBuffer, false, 0.8f);
	}

	HRESULT playSpaceCraftDestructionSound() {
		return playAudio(pSpaceCraftDestructionVoice, spaceCraftDestructionBuffer, false, 0.7f);
	}

	HRESULT playSaucerSound() {
		return playAudio(pSaucerVoice, saucerBuffer, true);
	}

	HRESULT stopSaucerSound() {
		if (pSaucerVoice != nullptr && FAILED(hr = pSaucerVoice->Stop(0)))
			return hr;
		if (pSaucerVoice != nullptr && FAILED(hr = pSmallSaucerVoice->Stop(0)))
			return hr;
	}
	
	HRESULT playSmallSaucerSound() {
		return playAudio(pSmallSaucerVoice, smallSaucerBuffer, true);
	}

	HRESULT playShootingSound() {
		return playAudio(pShootingVoice, shootingBuffer, false, 0.5f);
	}

	HRESULT playThrustSound() {
		if (!this->thrusting) {
			this->thrusting = true;
			return playAudio(pThrustVoice, thrustBuffer, true);
		}
		
	}

	HRESULT stopThrustSound() {
		if (this->thrusting) {
			this->thrusting = false;
			if (FAILED(hr = pThrustVoice->Stop(0)))
				return hr;
		}
	}
};

