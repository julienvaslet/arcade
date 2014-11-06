#include <audio/PlayingSound.h>
#include <audio/Mixer.h>

#include <cmath>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace audio
{
	PlayingSound::PlayingSound( Sound * sound, unsigned int frequency, unsigned short int channels ) : sound(NULL), startDate(0), position(0), pitch(1.0), repeated(false), repeatedTimes(0), playing(false)
	{
		this->sound = new Sound( frequency, channels );
		//TODO: conversion
		this->sound->setData( sound->getData() );
	}
	
	PlayingSound::~PlayingSound()
	{
		if( this->sound != NULL )
			delete this->sound;
	}

	Sound * PlayingSound::getSound()
	{
		return this->sound;
	}
	
	unsigned int PlayingSound::getStartDate() const
	{
		return this->startDate;
	}
	
	unsigned int PlayingSound::getPosition() const
	{
		return this->position;
	}
	
	double PlayingSound::getPitch() const
	{
		return this->pitch;
	}
	
	bool PlayingSound::isRepeated() const
	{
		return this->repeated;
	}
	
	unsigned int PlayingSound::getRepeatedTimes()
	{
		return this->repeated ? this->repeatedTimes : 0;
	}
	
	bool PlayingSound::isPlaying() const
	{
		return this->playing;
	}
	
	void PlayingSound::play( unsigned int ticks )
	{
		this->startDate = ( ticks == 0 ) ? Mixer::getTicks() : ticks;
		this->playing = true;
	}

	void PlayingSound::setPosition( unsigned int position, bool relative )
	{
		if( relative )
			this->position += position;
		else
			this->position = position;
		
		if( this->position >= this->sound->getDataLength() )
		{
			// TODO: handle repeat times
			this->playing = false;
		}
	}
	
	void PlayingSound::setPitch( double pitch )
	{
		this->pitch = pitch;
	}
	
	void PlayingSound::setRepeat( bool repeat, unsigned int times )
	{
		this->repeated = repeat;
		this->repeatedTimes = times;
	}
	
	/*float PlayingSound::getTransitionVolume( unsigned int position )
	{
		float volume = 1.0;
		unsigned int deltaPosition = 0.1f * this->sound->getFrequency() * this->sound->getChannels();
		
		if( position < deltaPosition )
			volume = static_cast<double>( position ) / static_cast<double>( deltaPosition );
			
		else if( position >= this->sound->getDataLength() - deltaPosition )
			volume = static_cast<double>( this->sound->getDataLength() - position ) / static_cast<double>( deltaPosition );
		
		volume = (sin((volume * M_PI) - (M_PI/2.0f)) + 1.0f) / 2.0f;
		
		return volume;
	}*/
}

