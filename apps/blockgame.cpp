#include <SDL2/SDL.h>
#include <tools/logger/Stdout.h>

#include <opengl/Screen.h>
#include <opengl/OpenGL.h>
#include <opengl/BitmapFont.h>
#include <controller/Controller.h>

#include <audio/Mixer.h>

#include <blockgame/IntroScene.h>

using namespace opengl;
using namespace tools::logger;
using namespace audio;
using namespace controller;
using namespace blockgame;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( !Screen::initialize() )
	{
		Logger::get() << "Unable to initialize screen. Exiting.\n";
		return 1;
	}
	
	//new Mixer( 44100, 1, 256 );
	
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
	
	Screen::get()->setClearColor( Color( 0.0f, 0.0f, 0.0f, 0.0f ) );
	new BitmapFont( "data/fonts/bitmap.tga", 32, 32, 7, 1 );

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
	Screen::destroy();
	//Mixer::destroy();
	Logger::destroy();
	
	return 0;
}

