#include "audio.h"
#include <raylib.h>
#include <vector>
#include <cmath>
#include <asserts.h>
#include <settings.h>
#include <raymath.h>

namespace Audio {

	void init()
	{
		InitAudioDevice();
		SetMasterVolume(0.9);

		loadAllMusicAndSounds();
	}
	
	std::vector<Music> allMusic;
	int currentMusicPlaying = 0;

	std::vector<Sound> allSounds;

	void loadAllMusicAndSounds() {
		//an empty music at index 0
		allMusic.push_back({});

		//lamda
		auto loadMusic = [&](const char *path) {
			Music m;

			m = LoadMusicStream(path);

			if (m.stream.buffer) {
				m.looping = true;
				allMusic.push_back(m);
			}
			else {
				allMusic.push_back({});
			}
		};

		loadMusic(RESOURCES_PATH "music/forest.ogg");
		loadMusic(RESOURCES_PATH "music/desert.ogg");
		loadMusic(RESOURCES_PATH "music/snow.ogg");
		loadMusic(RESOURCES_PATH "music/cave.ogg");

		permaAssertComment(allMusic.size() == MUSIC_COUNT, "Forgot to add a song here!");

		//lamda
		auto loadSound = [&](const char *path) {
			Sound s;

			s = LoadSound(path);

			if (s.stream.buffer) {
				allSounds.push_back(s);
			}
			else {
				allSounds.push_back({}); // empty sound
			}
		};

		// an empty sound at index 0
		allSounds.push_back({});

		loadSound(RESOURCES_PATH "sounds/place.ogg");
		loadSound(RESOURCES_PATH "sounds/break.ogg");

		permaAssertComment(allSounds.size() == SOUNDS_COUNT, "Forgot to add a sound here!");
	}

	void playMusic(int index) {
		if(allMusic.size() <= index) { return; }
		if (currentMusicPlaying == index) { return; }

		StopMusicStream(allMusic[currentMusicPlaying]);
		PlayMusicStream(allMusic[index]);
		SetMusicVolume(allMusic[index], std::pow(getSettings().musicVolume * getSettings().masterVolume, 1.0));

		currentMusicPlaying = index;
	}

	void update() {
		if (!isMusicPlaying()) {
			currentMusicPlaying = 0;
			return;
		}

		SetMusicVolume(allMusic[currentMusicPlaying], std::pow(getSettings().musicVolume * getSettings().musicVolume, 1.0));
		UpdateMusicStream(allMusic[currentMusicPlaying]);
	}

	void stopAllMusic() {
		StopMusicStream(allMusic[currentMusicPlaying]);
		currentMusicPlaying = 0;
	}

	void playSound(int sound, float volume) {
		if (sound <= noneSound || sound >= SOUNDS_COUNT) { return; }

		// #include <raymath.h>
		volume = Clamp(volume, 0, 1);

		// Put the following commented out code if you find the volume is too loud. volume * volume will make it smaller because the values of volume are between 0 and 1
		//volume *= getSettings().masterVolume * getSettings().masterVolume;
		//volume *= getSettings().soundsVolume * getSettings().soundsVolume;

		volume *= getSettings().masterVolume;
		volume *= getSettings().soundsVolume;

		SetSoundVolume(allSounds[sound], volume);
		PlaySound(allSounds[sound]);
	}

	bool isMusicPlaying() {
		if (!currentMusicPlaying) { return 0; }
		return IsAudioStreamPlaying(allMusic[currentMusicPlaying].stream);
	}
}
