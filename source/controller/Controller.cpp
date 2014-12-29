#include <controller/Controller.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

#include <cstring>

namespace controller
{
	map<SDL_JoystickID, Controller *> Controller::controllers;
	map<string, Mapping *> Controller::mappings;
	
	Controller::Controller( SDL_Joystick * joystick, const string& mapping ) : joystick(joystick), id(0), mapping(NULL)
	{
		this->id = SDL_JoystickInstanceID( this->joystick );
		
		if( !this->loadMapping( mapping ) )
		{
			this->loadMapping( "generic" );
			
			#ifdef DEBUG0
			if( this->mapping == NULL )
				Logger::get() << "[Controller] No mapping found. Events will not be handled." << Logger::endl;
			#endif
		}
		
		#ifdef DEBUG0
		Logger::get() << "[Controller#" << this->id << "] \"" << SDL_JoystickName( this->joystick ) << "\" initialized." << Logger::endl;
		#endif
		
		//TODO: initialize button states
	}
	
	Controller::~Controller()
	{
		if( this->joystick != NULL )
		{
			SDL_JoystickClose( this->joystick );
			
			#ifdef DEBUG0
			Logger::get() << "[Controller#" << this->id << "] Closed." << Logger::endl;
			#endif
		}
	}
	
	bool Controller::loadMapping( const string& mapping )
	{
		bool status = true;
		
		map<string, Mapping *>::iterator itMapping = Controller::mappings.find( mapping );
		
		if( itMapping == Controller::mappings.end() )
		{
			this->mapping = new Mapping( mapping );
			status = this->mapping->isLoaded();
			
			if( status )
				Controller::mappings[mapping] = this->mapping;
				
			else
			{
				delete this->mapping;
				this->mapping = NULL;
			}
		}
		else
			this->mapping = itMapping->second;
			
		return status;
	}
	
	unsigned int Controller::getId() const
	{
		return static_cast<unsigned int>( this->id );
	}
	
	void Controller::scan()
	{
		#ifdef DEBUG0
		Logger::get() << "[Controller] Looking for available controllers..." << Logger::endl;
		#endif
		
		SDL_Joystick * joystick = NULL;
		
		for( int i = 0 ; i < SDL_NumJoysticks() ; i++ )
		{
			joystick = SDL_JoystickOpen( i );
			
			if( joystick != NULL )
			{
				SDL_JoystickID id = SDL_JoystickInstanceID( joystick );
				map<SDL_JoystickID, Controller *>::iterator it = Controller::controllers.find( id );
				
				if( it == Controller::controllers.end() )
					Controller::controllers[id] = new Controller( joystick, SDL_JoystickName( joystick ) );
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[Controller] Can not open #" << i << ": " << SDL_GetError() << Logger::endl;
			#endif
		}
	}
	
	bool Controller::open( const string& name )
	{
		return Controller::open( name, name );
	}
	
	bool Controller::open( const string& name, const string& mapping )
	{
		bool success = false;
		#ifdef DEBUG0
		Logger::get() << "[Controller] Looking for controller \"" << name << "\"..." << Logger::endl;
		#endif
		
		SDL_Joystick * joystick = NULL;
		
		for( int i = 0 ; i < SDL_NumJoysticks() ; i++ )
		{
			if( strncmp( name.c_str(), SDL_JoystickNameForIndex( i ), name.length() ) == 0 )
			{
				joystick = SDL_JoystickOpen( i );
				break;
			}
		}
		
		if( joystick != NULL )
		{
			SDL_JoystickID id = SDL_JoystickInstanceID( joystick );
			map<SDL_JoystickID, Controller *>::iterator it = Controller::controllers.find( id );
			
			if( it == Controller::controllers.end() )
			{
				Controller::controllers[id] = new Controller( joystick, mapping );
				success = true;
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[Controller] \"" << name << "\" is already opened." << Logger::endl;
			#endif
			
		}
		#ifdef DEBUG0
		else
			Logger::get() << "[Controller] \"" << name << "\" was not found." << Logger::endl;
		#endif
		
		return success;
	}
	
	void Controller::destroy()
	{
		for( map<SDL_JoystickID, Controller *>::iterator it = Controller::controllers.begin() ; it != Controller::controllers.end() ; it++ )
			delete it->second;
		
		Controller::controllers.clear();
		
		for( map<string, Mapping *>::iterator it = Controller::mappings.begin() ; it != Controller::mappings.end() ; it++ )
			delete it->second;
		
		Controller::mappings.clear();	
	}
	
	unsigned int Controller::getControllersCount()
	{
		return Controller::controllers.size();
	}
	
	void Controller::handleEvent( const SDL_Event * event )
	{
		map<SDL_JoystickID, Controller *>::iterator it = Controller::controllers.end();
		
		switch( event->type )
		{
			case SDL_JOYBUTTONUP:
			{
				it = Controller::controllers.find( event->jbutton.which );
				
				if( it != Controller::controllers.end() )
				{
					Logger::get() << "[Controller#" << it->second->getId() << "] Button up !" << Logger::endl;
				}
				
				break;
			}
			
			case SDL_JOYBUTTONDOWN:
			{
				it = Controller::controllers.find( event->jbutton.which );
				
				if( it != Controller::controllers.end() )
				{
					Logger::get() << "[Controller#" << it->second->getId() << "] Button down !" << Logger::endl;
				}
				
				break;
			}
			
			case SDL_JOYAXISMOTION:
			{
				it = Controller::controllers.find( event->jbutton.which );
				
				if( it != Controller::controllers.end() )
				{
					Logger::get() << "[Controller#" << it->second->getId() << "] Axis moved !" << Logger::endl;
				}
				
				break;
			}
			
			case SDL_JOYDEVICEREMOVED:
			{
				it = Controller::controllers.find( event->jbutton.which );
				
				if( it != Controller::controllers.end() )
				{
					delete it->second;
					Controller::controllers.erase( it );
				}
				
				break;
			}
		}
	}
}

