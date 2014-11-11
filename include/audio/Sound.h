#ifndef __AUDIO_SOUND_H
#define __AUDIO_SOUND_H	1

#include <SDL2/SDL.h>
#include <vector>

using namespace std;

namespace audio
{
	class Sound
	{
		protected:
			unsigned int frequency;
			unsigned short int channels;
			vector<int> data;
			
		public:
			Sound( unsigned int frequency = 44100, unsigned short int channels = 2 );
			virtual ~Sound();
			
			unsigned int getDuration() const;
			unsigned int getDataLength() const;
			unsigned int getFrequency() const;
			unsigned short int getChannels() const;
			
			Sound * getData( unsigned int startPosition, unsigned int length );
			int getData( unsigned int position ) const;
			int getData( double position ) const;
			const vector<int>& getData() const;
			void setData( unsigned int position, int value );
			void setData( const vector<int>& data );
			
			void mix( const Sound * sound, unsigned int startMs = 0, float volume = 1.0f, unsigned int fromMs = 0, unsigned int toMs = 0, double pitch = 1.0 );
			void rawMix( const Sound * sound, unsigned int startPosition = 0, float volume = 1.0f, unsigned int fromPosition = 0, unsigned int toPosition = 0, double pitch = 1.0 );
	};
}

#endif
