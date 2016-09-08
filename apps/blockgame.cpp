#include <SDL2/SDL.h>
#include <tools/logger/Stdout.h>

#include <opengl/Screen.h>
#include <opengl/OpenGL.h>
#include <opengl/ui/BitmapFont.h>
#include <controller/Controller.h>

#include <audio/Mixer.h>

#include <blockgame/IntroScene.h>
#include <blockgame/Constants.h>

using namespace opengl;
using namespace opengl::ui;
using namespace tools::logger;
using namespace audio;
using namespace controller;
using namespace blockgame;

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
	
	new Mixer( 44100, 1, 512 );
	
	Controller::open( "Joystick1" );
	
	if( Controller::getControllersCount() == 0 )
		Controller::scan();
		
	Controller * controller = Controller::getFreeController();
	
	if( controller != NULL )
	{
		new Player( "Player" );
		Player::get( "Player" )->setController( controller );
	}
	
	SDL_Event lastEvent;
	game::Scene * currentScene = NULL;
	
	new BitmapFont( "data/fonts/bitmap.tga", 32, 32, 7, 1 );	
	Resource::loadTexture2D( "texture.block", "data/blockgame/block.tga", true );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	currentScene = new IntroScene();

	while( currentScene != NULL )
	{
		while( currentScene->isRunning() )
		{
			while( SDL_PollEvent( &lastEvent ) )
				currentScene->handleEvent( &lastEvent );
	
			unsigned int ticks = SDL_GetTicks();
	
			currentScene->live( ticks );
			currentScene->render( ticks );
		}
	
		Scene * nextScene = currentScene->getNextScene();
	
		delete currentScene;
		currentScene = nextScene;
	}
	
	Player::destroy();
	Controller::destroy();
	Font::destroy();
	Resource::destroy();
	Screen::destroy();
	Mixer::destroy();
	Logger::destroy();
	
	return 0;
}

