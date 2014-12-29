#ifndef __CONTROLLER_CONTROLLER_H
#define __CONTROLLER_CONTROLLER_H	1

#include <SDL2/SDL.h>
#include <map>
#include <string>

#include <controller/Mapping.h>

using namespace std;

namespace controller
{
	class Controller
	{
		protected:
			static map<SDL_JoystickID, Controller *> controllers;
			static map<string, Mapping *> mappings;
			
			SDL_Joystick * joystick;
			SDL_JoystickID id;
			Mapping * mapping;
		
			Controller( SDL_Joystick * joystick, const string& mapping );
			virtual ~Controller();
			
			bool loadMapping( const string& mapping );
			
		public:
			static void scan();
			static void destroy();
			static bool open( const string& name );
			static bool open( const string& name, const string& mapping );
			static void handleEvent( const SDL_Event * event );
			static unsigned int getControllersCount();
			
			unsigned int getId() const;
	};
}

#endif

