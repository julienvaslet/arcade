#include <SDL2/SDL.h>

#include <opengl/Screen.h>
#include <opengl/TexturedRectangle.h>
#include <opengl/Matrix.h>
#include <game/Resource.h>

#include <controller/Controller.h>
#include <tools/logger/Stdout.h>

using namespace opengl;
using namespace tools::logger;
using namespace controller;

#define SCREEN_WIDTH	100
#define SCREEN_HEIGHT	100

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( !Screen::initialize( SCREEN_WIDTH, SCREEN_HEIGHT ) )
	{
		cout << "Unable to initialize screen. Exiting." << endl;
		return 1;
	}
	
	// Set the orthogonal origin at the top-left corner
	Matrix::projection = Matrix::ortho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1 );
	
	// Resources
	Resource::loadTexture2D( "texture0", "data/blockgame/block.tga" );
	
	TexturedRectangle * rectangle = new TexturedRectangle( 50, 50, "texture0" );
	rectangle->getOrigin().moveTo( 50.0f, 50.0f, 0.0f );
	rectangle->getAnchor().moveTo( 25.0f, 25.0f );
	rectangle->getTile()->setView( 0, 0, 128, 128 );
	
	// Load joysticks
	/*Controller::open( "Joystick1" );
	Controller::open( "Joystick2" );
	
	if( Controller::getControllersCount() == 0 )
		Controller::scan();*/
		
	vector<Point3D> vertices;
	vector<Point2D> textureCoordinates;
	vector<unsigned short int> indices;
	
	rectangle->prepareRendering( vertices, textureCoordinates, indices );
	
	bool running = true;
	SDL_Event lastEvent;
	unsigned int lastDrawTicks = 0;
	
	unsigned int lastRotate = 0;
	Matrix translation = Matrix::translation( rectangle->getOrigin().getX(), rectangle->getOrigin().getY(), rectangle->getOrigin().getZ() );
	Matrix reverseTranslation = Matrix::translation( -1 * rectangle->getOrigin().getX(), -1 * rectangle->getOrigin().getY(), -1 *rectangle->getOrigin().getZ() );
	Matrix rotate = Matrix::rotationZ( 1 );
	
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
		
		if( ticks - lastRotate > 5 )
		{
			Matrix::modelview.multiply( translation );
			Matrix::modelview.multiply( rotate );
			Matrix::modelview.multiply( reverseTranslation );
			
			lastRotate = ticks;
		}
		
		if( ticks - lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			
			TexturedRectangle::render( vertices, textureCoordinates, indices, rectangle->getTile()->getTexture(), 0 );
			
			Screen::get()->render();
			
			lastDrawTicks = ticks;
		}
	}
	
	delete rectangle;
	Resource::destroy();
	Controller::destroy();
	Screen::destroy();
	Logger::destroy();
	
	return 0;
}