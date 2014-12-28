#include <SDL2/SDL.h>
#include <iostream>

#include <opengl/Screen.h>
#include <opengl/OpenGL.h>
#include <tools/logger/Stdout.h>

using namespace opengl;
using namespace std;
using namespace tools::logger;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( !Screen::initialize() )
	{
		cout << "Unable to initialize screen. Exiting." << endl;
		return 1;
	}
	
	cout << "Vendor: " << OpenGL::getVendor() << endl;
	cout << "Renderer: " << OpenGL::getRenderer() << endl;
	cout << "Version: " << OpenGL::getVersion() << endl;
	cout << "Extensions: " << endl;
	
	set<string> extensions = OpenGL::getExtensions();
	for( set<string>::const_iterator it = extensions.begin() ; it != extensions.end() ; it++ )
		cout << "\t" << *it << endl;
	cout << endl;
		
	cout << "Has extension GL_EXT_texture: " << ( OpenGL::hasExtension( "GL_EXT_texture" ) ? "yes" : "no" ) << endl;
	
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
	
	Logger::destroy();
	
	return 0;
}
