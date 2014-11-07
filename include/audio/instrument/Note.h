#ifndef __AUDIO_INSTRUMENT_NOTE_H
#define __AUDIO_INSTRUMENT_NOTE_H 1

namespace audio
{
	namespace instrument
	{
		class Note
		{
			private:
				Note();
				~Note();
				
			public:
				static float getFrequency( char note, bool sharp = false, int octave = 4, float A4 = 440.00f );
		};
	}
}
				
#endif

