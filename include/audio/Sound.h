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
			
			int getData( unsigned int position ) const;
			const vector<int>& getData() const;
			void setData( unsigned int position, int value );
			void setData( const vector<int>& data );
	};
}

#endif
