#include <SDL2/SDL.h>
#include <iostream>

#include <opengl/Screen.h>
#include <opengl/OpenGL.h>

using namespace opengl;
using namespace std;

int main( int argc, char ** argv )
{
	if( !Screen::initialize( "003 - Vertex Buffer Object" ) )
	{
		cout << "Unable to initialize screen. Exiting." << endl;
		return 1;
	}
	
	bool running = true;
	SDL_Event lastEvent;
	unsigned int lastDrawTicks = 0;

	while( running )
	{
		while( SDL_PollEvent( &lastEvent ) )
		{
			switch( lastEvent.type )
			{
				case SDL_QUIT:
				{
					running = false;
					break;
				}
			}
		}

		unsigned int ticks = SDL_GetTicks();
		
		if( ticks - lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			
			Screen::get()->render();
			
			lastDrawTicks = ticks;
		}
	}

	Screen::destroy();
	
	return 0;
}
