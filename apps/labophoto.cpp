#include <SDL2/SDL.h>
#include <opengl/Screen.h>
#include <opengl/OpenGL.h>
#include <opengl/Matrix.h>
#include <game/Resource.h>

#include <opengl/TexturedRectangle.h>

#include <tools/logger/Stdout.h>
#include <tools/camera/GphotoCamera.h>

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

	// Set the orthogonal origin at the top-left corner
	Matrix::projection = Matrix::ortho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1 );
	Color backgroundColor( "cccccc" );
	Screen::get()->setClearColor( backgroundColor );
	
	GphotoCamera * camera = NULL;
	GphotoCamera::initializeContext();
	
	map<string,string> cameras;
	GphotoCamera::list( cameras );
	
	if( cameras.size() > 0 )
	{
		camera = GphotoCamera::getCamera( cameras.begin()->first );
	}
	else
		Logger::get() << "No camera detected." << Logger::endl;
		
	if( camera != NULL )
	{
		camera->capture( "capture.jpg" );
		
		bool running = true;
		SDL_Event lastEvent;
		unsigned int lastDrawTicks = 0;
	
		Resource::loadTexture2D( "photo", "capture.jpg", true );
		Texture2D * texture = (Texture2D *) Resource::get("photo");
		
		if( texture != NULL )
		{
			TexturedRectangle * photo = new TexturedRectangle( SCREEN_WIDTH, SCREEN_HEIGHT, texture );

			vector<Point3D> vertices;
			vector<Point2D> textureCoordinates;
			vector<unsigned short int> indices;

			photo->prepareRendering( vertices, textureCoordinates, indices );
			
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
				
				if( ticks - lastDrawTicks > 30 )
				{
					Screen::get()->clear();

					TexturedRectangle::render( vertices, textureCoordinates, indices, texture );
					Screen::get()->render();
					
					lastDrawTicks = ticks;
				}
			}
			
			delete photo;
		}
	}
	
	GphotoCamera::destroyContext();
	Resource::destroy();
	Screen::destroy();
	Logger::destroy();
	
	return 0;
}

