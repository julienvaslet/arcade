#include <SDL2/SDL.h>
#include <opengl/Screen.h>
#include <opengl/OpenGL.h>
#include <opengl/Matrix.h>
#include <opengl/BitmapFont.h>
#include <game/Resource.h>

#include <opengl/TexturedRectangle.h>

#include <tools/logger/Stdout.h>
#include <tools/camera/GphotoCamera.h>

#include <opengl/ui/UserInterface.h>
#include <opengl/ui/Button.h>

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

using namespace opengl;
using namespace game;
using namespace tools::logger;
using namespace tools::camera;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( !Screen::initialize( SCREEN_WIDTH, SCREEN_HEIGHT ) )
	{
		Logger::get() << "Unable to initialize screen. Exiting.\n";
		return 1;
	}
	
	new BitmapFont( "data/fonts/bitmap.tga", 32, 32, 7, 1 );
	
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	ui::UserInterface * ui = new ui::UserInterface();
	ui->setFont( "bitmap", 0.40f );
	
	ui::Button * btn = new ui::Button( "btn", "Click me!" );
	btn->getRectangle().getOrigin().moveTo( 100, 100, 0 );
	ui->addElement( btn );

	// Set the orthogonal origin at the top-left corner
	Matrix::projection = Matrix::ortho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1 );
	Color backgroundColor( "888888" );
	Screen::get()->setClearColor( backgroundColor );
		
	bool running = true;
	SDL_Event lastEvent;
	unsigned int lastDrawTicks = 0;
	
	while( running )
	{
		while( SDL_PollEvent( &lastEvent ) )
		{
			// Global test temp...
			if( !ui->dispatchEvent( &lastEvent ) )
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
		}

		unsigned int ticks = SDL_GetTicks();
		
		if( ticks - lastDrawTicks > 30 )
		{
			Screen::get()->clear();

			ui->render( ticks );
			Screen::get()->render();
			
			lastDrawTicks = ticks;
		}
	}
	
	delete ui;
	
	//GphotoCamera::destroyContext();
	Font::destroy();
	Resource::destroy();
	Screen::destroy();
	Logger::destroy();
	
	return 0;
}

