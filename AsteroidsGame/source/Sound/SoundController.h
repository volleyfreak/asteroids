#pragma once
#include <xaudio2.h>
#include <string>

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'


//XAudio2 implementation following the official Microsoft Tutorial (https://docs.microsoft.com/de-de/windows/win32/xaudio2/getting-started)
class SoundController
{
private: 
	HRESULT hr;
	IXAudio2* pXAudio2 = nullptr;
	bool thrusting = false;
	bool backgroundPlaying = false;

	IXAudio2SourceVoice* pBackgroundVoice;
	const char* backgroundFile = __TEXT("./res/sounds/Background.wav");
	XAUDIO2_BUFFER backgroundBuffer = { 0 };

	IXAudio2SourceVoice* pSpaceShipDestructionVoice;
	const char* spaceShipDestructionFile = __TEXT("./res/sounds/spaceShipDestruction.wav");
	XAUDIO2_BUFFER spaceShipDestructionBuffer = { 0 };

	IXAudio2SourceVoice* pDestructionVoice;
	const char* destructionFile = __TEXT("./res/sounds/Destruction.wav");
	XAUDIO2_BUFFER destructionBuffer = { 0 };

	IXAudio2SourceVoice* pSaucerDestructionVoice;
	XAUDIO2_BUFFER saucerDestructionBuffer = { 0 };

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

	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);

	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);

	HRESULT SetupSourceVoice(const char* fileLocation, IXAudio2SourceVoice*& pSourceVoice, XAUDIO2_BUFFER& buffer);

	HRESULT playAudio(IXAudio2SourceVoice*& pSourceVoice, XAUDIO2_BUFFER& buffer, bool loopAudio = false, float volume = 1.0f);

public:
	SoundController();
	~SoundController();

	//Initializes Source voices
	HRESULT Initialize();

	//Plays background sound loop
	HRESULT playBackgroundSound();

	//Changes speed of backgrouns sound loop
	HRESULT pitchBackgroundSound(float pitch);

	//Stops background sound loop
	HRESULT stopBackgroundSound();

	//Plays asteroid destruction sound
	HRESULT playAsteroidDestructionSound(float pitch);

	//Plays saucer destruction sound
	HRESULT playSaucerDestructionSound();

	//Plays space ship destruction sound
	HRESULT playspaceShipDestructionSound();

	//Plays saucer background sound loop
	HRESULT playSaucerSound();

	//Stops saucer background sound loop
	HRESULT stopSaucerSound();
	
	//Plays small saucer background sound loop
	HRESULT playSmallSaucerSound();

	//Plays shooting sound
	HRESULT playShootingSound();

	//Plays thrusting sound loop
	HRESULT playThrustSound();

	//Stops thrusting sound loop
	HRESULT stopThrustSound();

	//Stops all running sound loops
	HRESULT stopAllSoundLoops();
};

