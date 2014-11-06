#include <audio/Sound.h>

#include <climits>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace audio
{
	Sound::Sound( unsigned int frequency, unsigned short int channels ) : frequency(frequency), channels(channels)
	{
	}
	
	Sound::~Sound()
	{
	}
	
	unsigned int Sound::getDuration() const
	{
		return static_cast<unsigned int>( ( static_cast<double>( this->getDataLength() ) / static_cast<double>( this->frequency ) ) / static_cast<double>( this->channels ) );
	}
	
	unsigned int Sound::getDataLength() const
	{
		return this->data.size();
	}
	
	unsigned int Sound::getFrequency() const
	{
		return this->frequency;
	}
	
	unsigned short int Sound::getChannels() const
	{
		return this->channels;
	}
	
	int Sound::getData( unsigned int position ) const
	{
		int value = 0;
		
		if( position < this->getDataLength() )
			value = this->data[position];
		
		return value;
	}
	
	const vector<int>& Sound::getData() const
	{
		return this->data;
	}
	
	Sound * Sound::getData( unsigned int startPosition, unsigned int length )
	{
		if( startPosition > this->getDataLength() )
			startPosition = this->getDataLength();
		
		if( startPosition + length > this->getDataLength() )
			length = this->getDataLength() - startPosition;
		
		Sound * sound = new Sound( this->frequency, this->channels );
		vector<int> data;
		
		for( unsigned int i = startPosition ; i < startPosition + length ; i++ )
			data.push_back( this->data[i] );
			
		sound->setData( data );
		
		return sound;
	}
	
	void Sound::setData( unsigned int position, int value )
	{
		if( position < this->getDataLength() )
			this->data[position] = value;
	}
	
	void Sound::setData( const vector<int>& data )
	{
		this->data.clear();
		
		for( vector<int>::const_iterator it = data.begin() ; it != data.end() ; it++ )
			this->data.push_back( *it );
	}
	
	void Sound::rawMix( const Sound * sound, unsigned int startPosition, float volume, unsigned int fromPosition, unsigned toPosition )
	{
		// Dev test: considering that sound have the same format
		
		// Reserve sound data vector if it will be too small
		if( startPosition + (toPosition - fromPosition) > this->getDataLength() )
			this->data.resize( startPosition + (toPosition - fromPosition) );
		
		// Mixing values
		for( unsigned int i = 0 ; i < (toPosition - fromPosition) ; i++ )
			this->data[startPosition+i] = static_cast<unsigned int>( static_cast<double>( this->data[startPosition+i] ) / 2.0 + static_cast<double>( sound->data[fromPosition+i] ) / 2.0 * volume );
	}
	
	void Sound::mix( const Sound * sound, unsigned int startMs, float volume, unsigned int fromMs, unsigned int toMs )
	{
		// Dev test: considering that sound have the same format
		unsigned int start = static_cast<unsigned int>( static_cast<double>( this->frequency * this->channels * startMs ) / 1000.0 );
		unsigned int from = static_cast<unsigned int>( static_cast<double>( this->frequency * this->channels * fromMs ) / 1000.0 );
		unsigned int to = (toMs == 0) ? sound->getDataLength() : static_cast<unsigned int>( static_cast<double>( this->frequency * this->channels * toMs ) / 1000.0 );

		this->rawMix( sound, start, volume, from, to );
	}
}

