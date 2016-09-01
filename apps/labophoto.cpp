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
#include <opengl/ui/PushButton.h>
#include <opengl/ui/DropDownList.h>

#include <unistd.h>

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

using namespace opengl;
using namespace game;
using namespace tools::logger;
using namespace tools::camera;

bool buttonAction( ui::Element * element, const ui::event::Event * event )
{
	const ui::event::MouseEvent * e = reinterpret_cast<const ui::event::MouseEvent *>( event );
	
	if( e->getButton() == ui::event::MouseButton::Left )
	{
		if( element->getRectangle().getAnchor().getX() == 0.0f )
			element->getRectangle().getAnchor().moveTo( element->getRectangle().getWidth(), element->getRectangle().getHeight() );
		else
			element->getRectangle().getAnchor().moveTo( 0.0f, 0.0f );
	}
	
	return true;
}

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( !Screen::initialize( SCREEN_WIDTH, SCREEN_HEIGHT ) )
	{
		Logger::get() << "Unable to initialize screen. Exiting.\n";
		return 1;
	}
	
	// Review this, it should be handled inside of UserInterface
	// Set the orthogonal origin at the top-left corner
	Matrix::projection = Matrix::ortho( 0, Screen::get()->getWidth(), Screen::get()->getHeight(), 0, -1, 1 );
	
	new BitmapFont( "data/fonts/bitmap.tga", 32, 32, 7, 1 );
	
	// This should be activated by Font or BitmapFont
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	ui::UserInterface * ui = new ui::UserInterface();
	ui->setFont( "bitmap", 0.5f );
	
	ui::Button * btn = new ui::Button( "btn", "Click me!" );
	btn->getRectangle().getOrigin().moveTo( 100.0f, 100.0f, 0.0f );
	btn->setButtonColor( "668b8b" );
	btn->addEventHandler( "mouseup", buttonAction );
	ui->addElement( btn );

	ui::PushButton * btn2 = new ui::PushButton( "btn2", "Push me!" );
	btn2->getRectangle().getOrigin().moveTo( 200.0f, 200.0f, 0.0f );
	btn2->setButtonColor( "668b8b" );
	ui->addElement( btn2 );
	
	ui::DropDownList * list = new ui::DropDownList( "list", "Items" );
	list->getRectangle().getOrigin().moveTo( 35.0f, 170.0f, 0.0f );
	//list->setBackgroundColor( "668b8b" );
	list->setButtonColor( "668b8b" );
	list->addItem( "Item 1" );
	list->addItem( "Item 2", true );
	list->addItem( "Item 3" );
	ui->addElement( list );

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
		else
			usleep( 15 );
	}
	
	delete ui;
	
	//GphotoCamera::destroyContext();
	Font::destroy();
	Resource::destroy();
	Screen::destroy();
	Logger::destroy();
	
	return 0;
}

