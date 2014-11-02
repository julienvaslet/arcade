#include <audio/Mixer.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace audio
{
	Mixer * Mixer::instance = NULL;
	
	Mixer::Mixer( unsigned int samplingFrequency, unsigned short int channels, unsigned int samples ) : samplingFrequency(samplingFrequency), channels(channels), samples(samples)
	{
		Mixer::destroy();
		
		#ifdef DEBUG0
		Logger::get() << "[Mixer] Initializing (" << static_cast<int>( this->channels ) << " channels, " << this->samplingFrequency << " Hz, " << this->samples << " samples)." << Logger::endl;
		#endif
		
		SDL_AudioSpec desiredFormat;
		SDL_AudioSpec format;
	
		desiredFormat.format = AUDIO_S16SYS;
		desiredFormat.freq = this->samplingFrequency;
		desiredFormat.channels = this->channels;
		desiredFormat.samples = this->samples;
		desiredFormat.callback = Mixer::callback;
		desiredFormat.userdata = NULL;

		if( SDL_OpenAudio( &desiredFormat, &format ) < 0 )
		{
			#ifdef DEBUG0
		    Logger::get() << "[Mixer] Unable to open audio device: " << SDL_GetError() << Logger::endl;
		    Logger::get() << "[Mixer] This mixer should be destroyed." << Logger::endl;
		    #endif
		}
		else
		{
			this->samplingFrequency = format.freq;
			this->channels = format.channels;
			this->samples = format.samples;
			
			#ifdef DEBUG0
			Logger::get() << "[Mixer] Initialized (" << static_cast<int>( this->channels ) << " channels, " << this->samplingFrequency << " Hz, " << this->samples << " samples)." << Logger::endl;
			#endif
			
			Mixer::instance = this;
			SDL_PauseAudio( 0 );
		}
	}
	
	Mixer::~Mixer()
	{
		SDL_PauseAudio( 0 );
		
		for( map<string, PlayingSound *>::iterator it = this->sounds.begin() ; it != this->sounds.end() ; it++ )
		{
			delete it->second;
			
			#ifdef DEBUG0
			Logger::get() << "[Mixer] Deleted sound \"" << it->first << "\"." << Logger::endl;
			#endif
		}
		
		this->sounds.clear();
		
		#ifdef DEBUG0
		Logger::get() << "[Mixer] Closing..." << Logger::endl;
		#endif
		
		SDL_CloseAudio();
		
		#ifdef DEBUG0
		Logger::get() << "[Mixer] Destroyed." << Logger::endl;
		#endif
	}

	unsigned int Mixer::getSamplingFrequency() const
	{
		return this->samplingFrequency;
	}
	
	unsigned short int Mixer::getChannels() const
	{
		return this->channels;
	}

	void Mixer::add( const string& name, Sound * sound )
	{
		map<string, PlayingSound *>::iterator it = this->sounds.find( name );
		
		if( it != this->sounds.end() )
			delete it->second;
		
		this->sounds[name] = new PlayingSound( sound, this->samplingFrequency, this->channels );
		
		#ifdef DEBUG0
		Logger::get() << "[Mixer] Added sound \"" << name << "\" (" << sound->getDuration() << "s)." << Logger::endl;
		#endif
	}
	
	void Mixer::play( const string& name, unsigned int ticks )
	{
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
	}
	
	bool Mixer::isPlaying()
	{
		bool playing = false;
		
		for( map<string, PlayingSound *>::iterator it = this->sounds.begin() ; it != this->sounds.end() ; it++ )
		{
			if( it->second->isPlaying() )
			{
				playing = true;
				break;
			}
		}

		return playing;
	}
	
	void Mixer::callback( void * unused, Uint8 * stream, int len )
	{
		Mixer * mixer = Mixer::get();
		vector<int> waves( len, 0 );

		if( mixer != NULL )
		{
			// Development case, only one sound
			for( map<string, PlayingSound *>::iterator it = mixer->sounds.begin() ; it != mixer->sounds.end() ; it++ )
			{
				if( it->second->isPlaying() )
				{
					unsigned int position = it->second->getPosition();
					unsigned int tocopy = it->second->getSound()->getDataLength() - position > static_cast<unsigned int>( len ) ? static_cast<unsigned int>( len ) : it->second->getSound()->getDataLength() - position;

					for( unsigned int i = 0 ; i < tocopy ; i++ )
						waves[i] += it->second->getSound()->getData( position + i );

					it->second->setPosition( position + tocopy );
				}
			}
			
			// SDL Sound format
			for( unsigned int i = 0 ; i < static_cast<unsigned int>( len ) ; i++ )
				stream[i] = (Uint8) ( waves[i] >= 0 ) ? waves[i] : 255 + waves[i];
		}
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

