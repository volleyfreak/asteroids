#pragma once
#include <string>
#include <Windows.h>

class SoundController
{
private: 
	boolean playSpaceCraftSound = true;
	boolean playSaucerSound = false;
	boolean backGroundPlaying = false;
	LPCWSTR gameSoundFile = L"./res/sounds/Background.wav";
	LPCWSTR spaceCraftDestructionFile = L"./res/sounds/SpacecraftDestruction.wav";
	LPCWSTR asteroidDestructionFile = L"./res/sounds/AsteroidDestruction.wav";
	LPCWSTR saucerDestructionFile = L"./res/sounds/SaucerDestruction.wav";
	LPCWSTR saucerFile = L"./res/sounds/SaucerBackground.wav";
	int noBackgroundSound = -1;
public:
	//todo: play sound parallel (https://stackoverflow.com/questions/7676083/multi-channel-sound-with-winmm-many-waveoutopen)
	//todo: useDirectSound API
	//todo: shoot sound
	//todo: accelerate sound

	void playBackgroundSound() {
		if (!backGroundPlaying) {
			if (playSpaceCraftSound && noBackgroundSound-- < 0) {
 				bool playsound = PlaySoundW(gameSoundFile, NULL, SND_ASYNC);
				this->backGroundPlaying = true;				
			}
			else if (playSaucerSound && noBackgroundSound-- < 0) {
				bool playsound = PlaySoundW(saucerFile, NULL, SND_ASYNC);
				this->backGroundPlaying = true;				
			}
			
		}
		
	}

	void playSpaceCraftDestructionSound() {
		bool playsound = PlaySoundW(spaceCraftDestructionFile, NULL, SND_ASYNC);
		this->noBackgroundSound = 120;
		this->backGroundPlaying = false;
	}

	void playAsteroidDestructionSound() {
		bool playsound = PlaySoundW(asteroidDestructionFile, NULL, SND_ASYNC);
		this->noBackgroundSound = 5;
		this->backGroundPlaying = false;
	}

	void playSaucerDestructionSound() {
		bool playsound = PlaySoundW(saucerDestructionFile, NULL, SND_ASYNC);
		this->noBackgroundSound = 25;
		this->backGroundPlaying = false;
		this->switchToGameSound();
	}

	void switchToGameSound() {
		this->playSpaceCraftSound = true;
		this->playSaucerSound = false;
		this->backGroundPlaying = false;
	}
	void switchToSaucerSound() {
		this->playSaucerSound = true;
		this->playSpaceCraftSound = false;
		this->backGroundPlaying = false;
	}
};

