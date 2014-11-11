#include <audio/PlayingSound.h>
#include <audio/Mixer.h>

#include <cmath>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace audio
{
	PlayingSound::PlayingSound( Sound * sound, unsigned int frequency, unsigned short int channels ) : sound(NULL), startDate(0), position(0), pitch(1.0), repeated(false), repeatedTimes(0), playing(false), oneTimePlaying(false), oneTimePlayed(false)
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
		this->timesToRepeat = this->getRepeatedTimes() - 1;
	}

	void PlayingSound::setPosition( unsigned int position, bool relative )
	{
		if( relative )
			this->position += position;
		else
			this->position = position;
		
		if( this->position >= this->sound->getDataLength() )
		{
			this->position = 0;
			
			if( this->repeated )
			{
				if( this->repeatedTimes > 0 )
				{
					if( this->timesToRepeat == 0 )
					{
						this->playing = false;
						this->oneTimePlayed = true;
					}
					else
						timesToRepeat--;
				}	
			}
			else
			{
				this->playing = false;
				this->oneTimePlayed = true;
			}
		}
	}
		
	void PlayingSound::setOneTimePlaying( bool oneTimePlaying )
	{
		this->oneTimePlaying = oneTimePlaying;
	}
	
	bool PlayingSound::hasPlayedOneTime() const
	{
		return this->oneTimePlaying && this->oneTimePlayed;
	}
	
	void PlayingSound::setPitch( double pitch )
	{
		this->pitch = pitch;
	}
	
	void PlayingSound::setRepeat( bool repeat, unsigned int times )
	{
		this->repeated = repeat;
		this->repeatedTimes = times;
		
		if( this->isPlaying() )
			this->timesToRepeat = this->repeatedTimes;
	}
}

