#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <tools/logger/Stdout.h>

#include <opengl/Screen.h>
#include <opengl/OpenGL.h>
#include <opengl/Camera.h>
#include <opengl/ui/BitmapFont.h>
#include <opengl/Program.h>

#include <data/image/Image.h>

using namespace opengl;
using namespace opengl::ui;
using namespace std;
using namespace tools::logger;
using namespace data;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( !Screen::initialize() )
	{
		Logger::get() << "Unable to initialize screen. Exiting.\n";
		return 1;
	}
	
	bool running = true;
	SDL_Event lastEvent;
	unsigned int lastDrawTicks = 0;
	
	new BitmapFont( "data/fonts/bitmap.tga", 32, 32, 7, 1 );

	/*Camera camera;
	camera.getEye().moveTo( 0.0f, 1.25f, 4.0f );
	camera.getCenter().moveTo( 0.0f, 0.0f, 0.0f );*/
	
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
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

			Font::get("bitmap")->write( Point2D( 20, 20 ), text.str(), 1.0f );
			Font::get("bitmap")->render();
			
			Screen::get()->render();
			
			lastDrawTicks = ticks;
		}
	}
	
	glUseProgram( 0 );
	Font::destroy();
	Screen::destroy();
	Logger::destroy();
	
	return 0;
}
