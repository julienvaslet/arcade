#include <SDL2/SDL.h>

#include <opengl/Screen.h>
#include <tools/logger/Stdout.h>

using namespace opengl;
using namespace tools::logger;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	bool running = true;
	
	if( !Screen::initialize() )
	{
		cout << "Unable to initialize screen. Exiting." << endl;
		return 1;
	}
	
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
				
				case SDL_KEYDOWN:
				{
		            if( lastEvent.key.keysym.sym == SDLK_ESCAPE )
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
	Logger::destroy();
	
	return 0;
}
