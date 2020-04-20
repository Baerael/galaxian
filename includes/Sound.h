/*****************************************************************************
 Yihsiang Liow
 Copyright
 *****************************************************************************/

/*****************************************************************************

  This class allows you to play a wav file. Note that you need to download
  SDL_Mixer. 

  USAGE:
  Sound sound("mysound.wav");
  sound.play();
  SDL_Delay(5000);

*****************************************************************************/
#ifndef SOUND_H
#define SOUND_H

#include "SDL.h"
#include "SDL_mixer.h"

class Sound
{
public:
	Sound(const char * filename = NULL)
		: _on(true)
	{
		SDL_Init(SDL_INIT_AUDIO);
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,
                  MIX_DEFAULT_FORMAT,
                  MIX_DEFAULT_CHANNELS, 512);
		sample = Mix_LoadWAV(filename);
		if (sample == NULL) 
		{
			printf("Error in Sound: Mix_LoadWAV returns NULL. %s\n", Mix_GetError());
		}
	}

	~Sound()
	{
		Mix_HaltChannel(-1);
		Mix_CloseAudio();
		Mix_FreeChunk(sample);
		sample = NULL;
	}

	void on()
	{
		_on = true;
	}

	void off()
	{
		_on = false;
	}

	void play()
	{
		if (_on) Mix_PlayChannel(-1, sample, 0);
	}

private:
	Mix_Chunk * sample;
	bool _on;
};

/*****************************************************************************

  This class allows you to play a wav file. Note that you need to download
  SDL_Mixer. 

  USAGE:
  
  Music music("a.mid");

  music.play();
  SDL_Delay(5000);

  music.stop();
  SDL_Delay(100);

  music.play();
  SDL_Delay(5000);

  music.stop();
  SDL_Delay(100);

  music.load("b.mid");
  music.play();
  SDL_Delay(5000);
  
  music.stop();
  SDL_Delay(100);

*****************************************************************************/

class Music
{
public:
	Music(const char * filename = NULL)
		: _on(true)
	{
		SDL_Init(SDL_INIT_AUDIO);
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,
                  MIX_DEFAULT_FORMAT,
                  MIX_DEFAULT_CHANNELS, 512);
		sample = Mix_LoadMUS(filename);
		if (sample == NULL) 
		{
			printf("Error in Sound: Mix_LoadMUS returns NULL. %s\n", Mix_GetError());
		}
	}

	void load(const char * filename = NULL)
	{
		if (filename != NULL)
		{
			free();
			sample = Mix_LoadMUS(filename);
		}
	}

	void free()
	{
		if (sample != NULL)
		{
			Mix_HaltMusic();
			Mix_FreeMusic(sample);
			sample = NULL;
		}
	}

	~Music()
	{
		Mix_HaltChannel(-1);
		Mix_CloseAudio();
		free();
		sample = NULL;
	}

	void on()
	{
		_on = true;
		play();
	}

	void off()
	{
		Mix_HaltMusic();
		_on = false;
	}

	void play()
	{
		if (_on) Mix_PlayMusic(sample, -1);
	}

	void stop()
	{
		Mix_HaltMusic();
	}

private:
	Mix_Music * sample;
	bool _on;
};

#endif
