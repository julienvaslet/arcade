#ifndef __AUDIO_MIXER_H
#define __AUDIO_MIXER_H	1

#include <audio/PlayingSound.h>

#include <SDL2/SDL.h>
#include <string>
#include <map>

using namespace std;

namespace audio
{
	class Mixer
	{	
		protected:
			static Mixer * instance;
			static void convertStream( const vector<int>& inStream, Uint8 * outStream, SDL_AudioFormat format );
			static unsigned int getRealSamples( unsigned int samples, SDL_AudioFormat format );
			static void callback( void * userdata, Uint8 * stream, int len );
			
			SDL_AudioDeviceID device;
			SDL_AudioFormat format;
			unsigned int samplingFrequency;
			unsigned short int channels;
			unsigned int samples;
			
			map<string, PlayingSound *> sounds;
			
		public:
			Mixer( unsigned int samplingFrequency, unsigned short int channels, unsigned int samples );
			virtual ~Mixer();
			
			unsigned int getSamplingFrequency() const;
			SDL_AudioFormat getAudioFormat() const;
			unsigned short int getChannels() const;
			
			// TODO: handle deletion on end for one-time sounds
			void add( const string& name, Sound * sound );
			void play( const string& name, unsigned int ticks = 0 );
			bool isPlaying();
			
			static Mixer * get();
			static void destroy();
			static unsigned int getTicks();
	};
}

#endif
