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
#include <opengl/Program.h>

using namespace opengl;
using namespace std;
using namespace tools::logger;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( !Screen::initialize( "008 - Font" ) )
	{
		Logger::get() << "Unable to initialize screen. Exiting.\n";
		return 1;
	}
	
	bool running = true;
	SDL_Event lastEvent;
	unsigned int lastDrawTicks = 0;
	
	new BitmapFont( "data/fonts/bitmap.bmp", 16, 16, 4, 1 );

	Camera camera;
	camera.getEye().moveTo( 0.0f, 1.25f, 4.0f );
	camera.getCenter().moveTo( 0.0f, 0.0f, 0.0f );
	
	glEnable( GL_DEPTH_TEST );
	
	Screen::get()->setClearColor( Color( 0.2f, 0.2f, 0.2f, 1.0f ) );
	
	stringstream text;
	
	for( int j = 2 ; j < 8 ; j++ )
	{
		for( int i = 0 ; i < 16 ; i++ )
		{
			char c = static_cast<char>( 16 * j + i );
			if( c == '\n' ) c = ' ';
			text << c;
		}
		
		text << '\n';
	}
	
	cout << endl << "Font must be shown as :" << endl;
	cout << text.str() << endl;
	
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
			camera.setPerspective( 45.0f, 800.0f / 600.0f, 1.0f, 100.0f );
			
			glMatrixMode( GL_MODELVIEW );
			camera.look();
			
			Font::get("data/fonts/bitmap.bmp")->render( Point2D( 20, Screen::get()->getHeight() - 40 ), text.str(), 2.0f );
			
			Screen::get()->render();
			
			lastDrawTicks = ticks;
		}
	}

	Font::destroy();
	Screen::destroy();
	Logger::destroy();
	
	return 0;
}
