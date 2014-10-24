#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <tools/logger/Stdout.h>

#include <opengl/Screen.h>
#include <opengl/OpenGL.h>
#include <opengl/Camera.h>
#include <opengl/BitmapFont.h>

using namespace opengl;
using namespace std;
using namespace tools::logger;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( !Screen::initialize( "Blockgame" ) )
	{
		Logger::get() << "Unable to initialize screen. Exiting.\n";
		return 1;
	}
	
	bool running = true;
	SDL_Event lastEvent;
	unsigned int lastDrawTicks = 0;
	
	new BitmapFont( "data/fonts/bitmap.bmp", 32, 32, 7, 1 );

	Camera camera;
	camera.getEye().moveTo( 0.0f, 0.0f, 0.0f );
	camera.getCenter().moveTo( 0.0f, 0.0f, 0.0f );
	
	Screen::get()->setClearColor( Color( 0.5f, 0.5f, 0.5f, 1.0f ) );
	
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
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
			camera.setPerspective( 45.0f, static_cast<float>( Screen::get()->getWidth() ) / static_cast<float>( Screen::get()->getHeight() ), 1.0f, 100.0f );
			
			glMatrixMode( GL_MODELVIEW );
			camera.look();
			
			// User Interface			
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 80 ), "Score", 1.0f );
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 111 ), "104903", 1.0f );
			
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 160 ), "Lines", 1.0f );
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 191 ), "10", 1.0f );
			
			Screen::get()->render();
			
			lastDrawTicks = ticks;
		}
	}

	Font::destroy();
	Screen::destroy();
	Logger::destroy();
	
	return 0;
}

