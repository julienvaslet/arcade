#include <audio/Mixer.h>
#include <climits>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace audio
{
	Mixer * Mixer::instance = NULL;
	
	Mixer::Mixer( unsigned int samplingFrequency, unsigned short int channels, unsigned int samples ) : device(0), format(0), samplingFrequency(samplingFrequency), channels(channels), samples(samples)
	{
		Mixer::destroy();
		
		#ifdef DEBUG0
		Logger::get() << "[Mixer] Initializing (" << static_cast<int>( this->channels ) << " channels, " << this->samplingFrequency << " Hz, " << this->samples << " samples)." << Logger::endl;
		#endif
		
		SDL_AudioSpec desiredFormat;
		SDL_AudioSpec obtainedFormat;
	
		desiredFormat.format = AUDIO_S32SYS;
		desiredFormat.freq = this->samplingFrequency;
		desiredFormat.channels = this->channels;
		desiredFormat.samples = this->samples;
		desiredFormat.callback = Mixer::callback;
		desiredFormat.userdata = this;
		
		this->device = SDL_OpenAudioDevice( NULL, 0, &desiredFormat, &obtainedFormat, 0 );

		if( this->device > 0 )
		{
			this->samplingFrequency = obtainedFormat.freq;
			this->channels = obtainedFormat.channels;
			this->samples = obtainedFormat.samples;
			this->format = obtainedFormat.format;
			
			#ifdef DEBUG0
			Logger::get() << "[Mixer] Initialized (" << static_cast<int>( this->channels ) << " channels, " << this->samplingFrequency << " Hz, " << this->samples << " samples)." << Logger::endl;
			#endif
			
			Mixer::instance = this;
			SDL_PauseAudioDevice( this->device, 0 );
		}
		#ifdef DEBUG0
		else
		{
			const char * error = SDL_GetError();
		    Logger::get() << "[Mixer] Unable to open audio device: " << error << Logger::endl;
		    Logger::get() << "[Mixer] This mixer should be destroyed." << Logger::endl;
		}
	    #endif
	}
	
	Mixer::~Mixer()
	{
		if( this->device > 0 )
			SDL_PauseAudioDevice( this->device, 1 );
		
		for( map<string, PlayingSound *>::iterator it = this->sounds.begin() ; it != this->sounds.end() ; it++ )
		{
			delete it->second;
			
			#ifdef DEBUG0
			Logger::get() << "[Mixer] Deleted sound \"" << it->first << "\"." << Logger::endl;
			#endif
		}
		
		this->sounds.clear();

		if( this->device > 0 )
		{		
			#ifdef DEBUG0
			Logger::get() << "[Mixer] Closing..." << Logger::endl;
			#endif
		
			SDL_CloseAudioDevice( this->device );
		
			#ifdef DEBUG0
			Logger::get() << "[Mixer] Destroyed." << Logger::endl;
			#endif
		}
	}

	unsigned int Mixer::getSamplingFrequency() const
	{
		return this->samplingFrequency;
	}
	
	unsigned short int Mixer::getChannels() const
	{
		return this->channels;
	}
	
	SDL_AudioFormat Mixer::getAudioFormat() const
	{
		return this->format;
	}

	void Mixer::add( const string& name, Sound * sound, bool oneTimePlaying )
	{
		if( this->device > 0 )
		{
			SDL_LockAudioDevice( this->device );
		
			map<string, PlayingSound *>::iterator it = this->sounds.find( name );
		
			if( it != this->sounds.end() )
				delete it->second;
				
			PlayingSound * playingSound = new PlayingSound( sound, this->samplingFrequency, this->channels );
			playingSound->setOneTimePlaying( oneTimePlaying );
		
			this->sounds[name] = playingSound;
		
			#ifdef DEBUG0
			Logger::get() << "[Mixer] Added sound \"" << name << "\" (" << (sound->getDuration() / 1000.0f ) << "s)." << Logger::endl;
			#endif
		
			SDL_UnlockAudioDevice( this->device );
		}
	}
	
	void Mixer::play( const string& name, unsigned int ticks )
	{
		if( this->device > 0 )
		{
			SDL_LockAudioDevice( this->device );
		
			map<string, PlayingSound *>::iterator it = this->sounds.find( name );
		
			if( it != this->sounds.end() )
			{
				#ifdef DEBUG0
				Logger::get() << "[Mixer] Playing sound \"" << name << "\"." << Logger::endl;
				#endif
			
				it->second->play( ticks );
			}
			#ifdef DEBUG0
			else
			{
				Logger::get() << "[Mixer] Can not find sound \"" << name << "\" ; sound will not be played." << Logger::endl;
			}
			#endif
		
			SDL_UnlockAudioDevice( this->device );
		}
	}
	
	void Mixer::stop( const string& name )
	{
		if( this->device > 0 )
		{
			SDL_LockAudioDevice( this->device );
		
			map<string, PlayingSound *>::iterator it = this->sounds.find( name );
		
			if( it != this->sounds.end() )
			{
				#ifdef DEBUG0
				Logger::get() << "[Mixer] Stopping sound \"" << name << "\"." << Logger::endl;
				#endif
			
				it->second->stop();
			}
			#ifdef DEBUG0
			else
			{
				Logger::get() << "[Mixer] Can not find sound \"" << name << "\" ; sound will not be stopped." << Logger::endl;
			}
			#endif
		
			SDL_UnlockAudioDevice( this->device );
		}
	}
	
	void Mixer::setRepeat( const string& name, bool repeat, unsigned int times )
	{
		if( this->device > 0 )
		{
			SDL_LockAudioDevice( this->device );
		
			map<string, PlayingSound *>::iterator it = this->sounds.find( name );
		
			if( it != this->sounds.end() )
			{
				it->second->setRepeat( repeat, times );
				
				#ifdef DEBUG0
				if( repeat )
				{
					if( times > 0 )
						Logger::get() << "[Mixer] Enabled repeat for sound \"" << name << "\" for " << times << " times." << Logger::endl;
					else
						Logger::get() << "[Mixer] Enabled unlimited repeat for sound \"" << name << "\"." << Logger::endl;
				}
				else
					Logger::get() << "[Mixer] Repeat disabled for sound \"" << name << "\"." << Logger::endl;
				#endif
			}
			
			SDL_UnlockAudioDevice( this->device );
		}
	}
	
	void Mixer::setPitch( const string& name, double pitch )
	{
		if( this->device > 0 )
		{
			SDL_LockAudioDevice( this->device );
		
			map<string, PlayingSound *>::iterator it = this->sounds.find( name );
		
			if( it != this->sounds.end() )
			{
				it->second->setPitch( pitch );
				
				#ifdef DEBUG0
				Logger::get() << "[Mixer] Pitch set to " << pitch << " for sound \"" << name << "\"." << Logger::endl;
				#endif
			}
			
			SDL_UnlockAudioDevice( this->device );
		}
	}
	
	bool Mixer::isPlaying()
	{
		bool playing = false;
		
		if( this->device > 0 )
		{
			SDL_LockAudioDevice( this->device );
		
			for( map<string, PlayingSound *>::iterator it = this->sounds.begin() ; it != this->sounds.end() ; it++ )
			{
				if( it->second->isPlaying() )
				{
					playing = true;
					break;
				}
			}
		
			SDL_UnlockAudioDevice( this->device );
		}

		return playing;
	}
	
	void Mixer::clean()
	{
		if( this->device > 0 )
		{
			SDL_LockAudioDevice( this->device );
		
			vector<string> soundsToErase;
		
			for( map<string, PlayingSound *>::iterator it = this->sounds.begin() ; it != this->sounds.end() ; it++ )
			{
				if( it->second->hasPlayedOneTime() )
					soundsToErase.push_back( it->first );
			}
			
			for( vector<string>::iterator it = soundsToErase.begin() ; it != soundsToErase.end(); it++ )
			{
				delete (this->sounds[*it]);
				this->sounds.erase( *it );
			}
		
			SDL_UnlockAudioDevice( this->device );
		}
	}
	
	void Mixer::callback( void * userdata, Uint8 * stream, int len )
	{
		Mixer * mixer = static_cast<Mixer *>( userdata );
		unsigned int realLength = Mixer::getRealSamples( len, mixer->getAudioFormat() );
		bool firstSound = true;
		vector<int> waves( realLength, 0.0 );
		
		Sound sound( mixer->samplingFrequency, mixer->channels );
		sound.setRawData( waves );

		if( mixer != NULL )
		{
			for( map<string, PlayingSound *>::iterator it = mixer->sounds.begin() ; it != mixer->sounds.end() ; it++ )
			{
				if( it->second->isPlaying() )
				{
					unsigned int position = it->second->getPosition();
					unsigned int pitchedLength = realLength * it->second->getPitch();
					unsigned int tocopy = ( position + pitchedLength > it->second->getSound()->getRawLength() ) ? it->second->getSound()->getRawLength() - position : pitchedLength;

					if( firstSound )
					{
						sound.rawMix( it->second->getSound(), 0, 2.0f, position, position + tocopy, it->second->getPitch() );
						firstSound = false;
					}
					else
						sound.rawMix( it->second->getSound(), 0, 1.0f, position, position + tocopy, it->second->getPitch() );
					
					it->second->setPosition( tocopy, true );
				}
			}

			vector<int> waves = sound.getRawData();
			Mixer::convertStream( waves, stream, mixer->getAudioFormat() );			
		}
	}
	
	unsigned int Mixer::getRealSamples( unsigned int samples, SDL_AudioFormat format )
	{
		// Base audio format size is 8 for Uint8
		return 8.0f / static_cast<float>( SDL_AUDIO_BITSIZE( format ) ) * samples;
	}
		
	void Mixer::convertStream( const vector<int>& inStream, Uint8 * outStream, SDL_AudioFormat format )
	{
		// Convert format (format is ignored)
		// See https://wiki.libsdl.org/SDL_AudioFormat
		Uint32 * stream = (Uint32 *) outStream;
		
		for( unsigned int i = 0 ; i < inStream.size() ; i++ )
			stream[i] = inStream[i];
	}
	
	Mixer * Mixer::get()
	{
		return Mixer::instance;
	}
	
	void Mixer::destroy()
	{
		if( Mixer::instance != NULL )
		{
			delete Mixer::instance;
			Mixer::instance = NULL;
		}
	}
	
	unsigned int Mixer::getTicks()
	{
		return SDL_GetTicks();
	}
}

