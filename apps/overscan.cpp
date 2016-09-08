#include <SDL2/SDL.h>

#include <opengl/Screen.h>
#include <opengl/ui/BitmapFont.h>
#include <game/Resource.h>
#include <game/Player.h>
#include <overscan/ConfigScene.h>

#include <controller/Controller.h>
#include <tools/logger/Stdout.h>
#include <tools/pi/Overscan.h>

#include <fstream>
#include <sstream>
#include <string>

using namespace opengl;
using namespace opengl::ui;
using namespace tools::logger;
using namespace tools::pi;
using namespace controller;
using namespace overscan;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( !Overscan::initialize() )
	{
		Logger::get() << "Could not initialize overscan tool. Exiting." << Logger::endl;

		Logger::destroy();
		exit( 1 );
	}
	
	if( !Screen::initialize( 0, 0, false, true ) )
	{
		Logger::get() << "Unable to initialize screen. Exiting." << Logger::endl;
		
		Overscan::destroy();
		Logger::destroy();
		exit( 1 );
	}
	
	// Set the orthogonal origin at the top-left corner
	Matrix::projection = Matrix::ortho( 0, Screen::get()->getWidth(), Screen::get()->getHeight(), 0, -1, 1 );
	
	new BitmapFont( "data/fonts/bitmap.tga", 32, 32, 7, 1 );
	
	// Load joysticks
	Controller::open( "Joystick1" );
	Controller::open( "Joystick2" );
	
	if( Controller::getControllersCount() == 0 )
		Controller::scan();
	
	Controller * controller = NULL;
	
	do
	{
		controller = Controller::getFreeController();
		
		if( controller != NULL )
		{
			Player * player = new Player( controller->getName() );
			player->setController( controller );
		}
	}
	while( controller != NULL );
		
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	SDL_Event lastEvent;
	Scene * currentScene = new ConfigScene();
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
	
	#ifdef __NO_X_WINDOW__
	// Save overscan (only on a pi)
	int left = 0;
	int right = 0;
	int top = 0;
	int bottom = 0;
	
	Overscan::get( &left, &right, &top, &bottom );
	
	stringstream ss;
	ss << "\n# Overscan configuration\n";
	ss << "overscan_left=" << left << "\n";
	ss << "overscan_right=" << right << "\n";
	ss << "overscan_top=" << top << "\n";
	ss << "overscan_bottom=" << bottom << "\n";
	
	ofstream configTxt;
	configTxt.open( "/boot/config.txt", ios::out | ios::app );
	
	if( configTxt.is_open() )
	{
		configTxt << ss.str();
		configTxt.close();
	}
	else
		Logger::get() << "Could not open \"/boot/config.txt\". Overscan configuration will not persist." << Logger::endl;

	#endif
	
	Player::destroy();
	Controller::destroy();
	Font::destroy();
	Resource::destroy();
	Screen::destroy();
	Overscan::destroy();
	Logger::destroy();
	
	return 0;
}
