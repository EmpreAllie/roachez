#include "roachez.h"

CSound:: CSound()
{
	m_Type = "CSound";
   int AudioRate = MIX_DEFAULT_FREQUENCY;
   uint16_t AudioFormat = MIX_DEFAULT_FORMAT;
   int AudioChannels = MIX_DEFAULT_CHANNELS;
   int AudioBuffers = 4096;
   int AudioVolume = MIX_MAX_VOLUME;

   if (Mix_OpenAudio(AudioRate, AudioFormat, AudioChannels, AudioBuffers) < 0) 
   {
      Loge("Couldn't open audio: %s\n", SDL_GetError());
      IsInitialized = false;
   } 
   else 
   {
      Mix_QuerySpec(&AudioRate, &AudioFormat, &AudioChannels);
      Logi("Opened audio at %d Hz %d bit%s %s %d bytes audio buffer\n", AudioRate,
         (AudioFormat&0xFF),
         (SDL_AUDIO_ISFLOAT(AudioFormat) ? " (float)" : ""),
         (AudioChannels > 2) ? "surround" : (AudioChannels > 1) ? "stereo" : "mono",
         AudioBuffers);
      Mix_VolumeMusic(AudioVolume);
   }
}

CSound:: ~CSound()
{
	for (auto s: m_Sounds)
	{
		Mix_FreeMusic(s);
	}

	Mix_CloseAudio();
}

void CSound:: playSound (ESound EnumSound)
{
   if (m_Sounds[EnumSound]) Mix_PlayMusic(m_Sounds[EnumSound], 0);

//#define TEST_MUSIC
#if defined TEST_MUSIC

   const char* typ;
   switch (Mix_GetMusicType(m_Sounds[EnumSound])) {
   case MUS_CMD:
       typ = "CMD";
       break;
   case MUS_WAV:
       typ = "WAV";
       break;
   case MUS_MOD:
   case MUS_MODPLUG_UNUSED:
       typ = "MOD";
       break;
   case MUS_FLAC:
       typ = "FLAC";
       break;
   case MUS_MID:
       typ = "MIDI";
       break;
   case MUS_OGG:
       typ = "OGG Vorbis";
       break;
   case MUS_MP3:
   case MUS_MP3_MAD_UNUSED:
       typ = "MP3";
       break;
   case MUS_OPUS:
       typ = "OPUS";
       break;
   case MUS_NONE:
   default:
       typ = "NONE";
       break;
   }

   Logi("Detected music type: %s", typ);
   Logi("Playing sound, duration %f\n", Mix_MusicDuration(m_Sounds[EnumSound]));

#endif

}

void CSound:: initialize()
{
	m_Sounds[SOUND_SLAP] = Mix_LoadMUS("./Audio/slap.flac");
	m_Sounds[SOUND_SMASH] = Mix_LoadMUS("./Audio/smash.flac");
	m_Sounds[SOUND_DING] = Mix_LoadMUS("./Audio/ding.flac");
	m_Sounds[SOUND_TYPING] = Mix_LoadMUS("./Audio/typing.flac");
	m_Sounds[SOUND_SMASH_GOLDEN] = Mix_LoadMUS("./Audio/smash-golden.flac");
}