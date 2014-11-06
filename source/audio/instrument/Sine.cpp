#include <audio/instrument/Sine.h>
#include <cmath>

namespace audio
{
	namespace instrument
	{
		Sine::Sine( unsigned int samplingFrequency, unsigned short int channels ) : Instrument(samplingFrequency, channels)
		{
		}
	
		Sine::~Sine()
		{
		}
		
		Sound * Sine::generate( float note, unsigned int duration, float volume )
		{
			Sound * sound = new Sound( this->samplingFrequency, this->channels );
			
			vector<int> noteData;
			double period = static_cast<double>( this->samplingFrequency ) / note;
	
			unsigned int i = 0;

			for( i = 0 ; i < this->samplingFrequency * duration / 1000.0 ; i++ )
			{
				unsigned iDegrees = 360.0 * ((i % static_cast<int>(period)) / period);
				int value = sin( ((iDegrees % 360) / 360.0) * 2.0 * M_PI ) * INT_MAX * volume;
		
				for( unsigned short int c = 0 ; c < this->channels ; c++ )
					noteData.push_back( value );
			}
	
			sound->setData( noteData );
			return sound;
		}
	}
}

