#include <SDL2/SDL.h>
#include <GLES2/gl2.h>

#include <tools/logger/Stdout.h>

using namespace tools::logger;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	bool running = true;
	
	SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO );
	
	/*
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, minorVersion );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );*/

	SDL_Window * window = SDL_CreateWindow(
		"Blackscreen Test",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_OPENGL
	);
	
	if( window == NULL )
	{
		Logger::get() << "[Screen] Unable to create window: " << SDL_GetError() << Logger::endl;
		running = false;
	}
	
	SDL_GLContext context = SDL_GL_CreateContext( window );
	
	if( context == NULL )
	{
		Logger::get() << "[Screen] Unable to create the context: " << SDL_GetError() << Logger::endl;
		running = false;
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
			glClearColor( 0.5f, 0.5f, 0.5f, 0.0f );
			glClear( GL_COLOR_BUFFER_BIT );
			/*glLoadIdentity();*/
			
			SDL_GL_SwapWindow( window );
			lastDrawTicks = ticks;
		}
	}

	SDL_GL_DeleteContext( context );
	SDL_DestroyWindow( window );
	
	SDL_Quit();
	
	Logger::destroy();
	
	return 0;
}
