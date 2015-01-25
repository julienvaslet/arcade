#include <SDL2/SDL.h>

#include <opengl/Screen.h>
#include <opengl/BitmapFont.h>
#include <game/Resource.h>
#include <game/Player.h>
#include <pong/PlayScene.h>

#include <controller/Controller.h>
#include <audio/Mixer.h>
#include <tools/logger/Stdout.h>

using namespace opengl;
using namespace tools::logger;
using namespace controller;
using namespace audio;
using namespace pong;

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( !Screen::initialize( SCREEN_WIDTH, SCREEN_HEIGHT ) )
	{
		cout << "Unable to initialize screen. Exiting." << endl;
		return 1;
	}
	
	new Mixer( 44100, 1, 256 );
	
	// Set the orthogonal origin at the top-left corner
	Matrix::projection = Matrix::ortho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1 );
	
	new BitmapFont( "data/fonts/bitmap.tga", 32, 32, 7, 1 );
	
	// Load joysticks
	Controller::open( "Joystick1" );
	Controller::open( "Joystick2" );
	
	if( Controller::getControllersCount() == 0 )
		Controller::scan();
	
	// Initializing first player
	Controller * controller = Controller::getFreeController();
	
	if( controller != NULL )
	{
		new Player( "Player1" );
		Player::get( "Player1" )->setController( controller );
	}
	
	// Initializing second player
	controller = Controller::getFreeController();
	
	if( controller != NULL )
	{
		new Player( "Player2" );
		Player::get( "Player2" )->setController( controller );
	}
	
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	SDL_Event lastEvent;
	Scene * currentScene = new PlayScene();

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
