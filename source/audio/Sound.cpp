#include <audio/Sound.h>

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
	
	void play();
}

