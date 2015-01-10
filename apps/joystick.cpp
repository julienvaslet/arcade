#include <SDL2/SDL.h>

#include <opengl/Screen.h>
#include <opengl/Point2D.h>
#include <opengl/BitmapFont.h>

#include <controller/Controller.h>
#include <tools/logger/Stdout.h>

#include <sstream>

using namespace opengl;
using namespace tools::logger;
using namespace controller;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( !Screen::initialize() )
	{
		cout << "Unable to initialize screen. Exiting." << endl;
		return 1;
	}
	
	new BitmapFont( "data/fonts/bitmap.tga", 32, 32, 7, 1 );
	
	// Load joysticks
	Controller::open( "Joystick1" );
	Controller::open( "Joystick2" );
	
	if( Controller::getControllersCount() == 0 )
		Controller::scan();
	
	bool running = true;
	SDL_Event lastEvent;
	unsigned int lastDrawTicks = 0;
	Controller * controller = NULL;
	stringstream text;
	
	unsigned int currentButton = 0;
	vector<string> buttons;
	buttons.push_back( "NorthButton" );
	buttons.push_back( "EastButton" );
	buttons.push_back( "SouthButton" );
	buttons.push_back( "WestButton" );
	buttons.push_back( "HorizontalAxis" );
	buttons.push_back( "VerticalAxis" );
	
	map<string, unsigned int> buttonValues;
	
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
				
				case SDL_JOYBUTTONDOWN:
				{
					if( controller == NULL )
						controller = Controller::get( lastEvent.jbutton.which );
						
					else if( static_cast<unsigned int>( lastEvent.jbutton.which ) == controller->getId() )
					{
						if( currentButton < buttons.size() )
						{
							buttonValues[buttons[currentButton]] = lastEvent.jbutton.button;
							currentButton++;
						}
						else
						{
							// End.
							running = false;
						}
					}
					
					break;
				}
				
				case SDL_JOYAXISMOTION:
				{
					if( controller == NULL )
						controller = Controller::get( lastEvent.jaxis.which );

					else if( static_cast<unsigned int>( lastEvent.jaxis.which ) == controller->getId() )
					{
						if( lastEvent.jaxis.value >= Mapping::Pushed || lastEvent.jaxis.value <= Mapping::ReversePushed )
						{
							if( currentButton == 0 || buttonValues[buttons[currentButton - 1]] != lastEvent.jaxis.axis )
							{
								if( currentButton < buttons.size() )
								{
									buttonValues[buttons[currentButton]] = lastEvent.jaxis.axis;
									currentButton++;
								}
								else
								{
									// End.
									running = false;
								}
							}
						}
					}
					
					break;
				}
				
				case SDL_JOYDEVICEREMOVED:
				{
					break;
				}
				
				case SDL_JOYDEVICEADDED:
				{
					Controller::scan();
					break;
				}
			}
		}

		unsigned int ticks = SDL_GetTicks();
		
		if( ticks - lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			Point2D origin( 0.0f, 0.0f );
			text.str( "" );
			
			if( controller == NULL )
				text << "Press any button to identify controller";
			else
			{
				text << "[" << controller->getName() << "]\n";
				
				for( unsigned int i = 0 ; i < buttons.size() ; i++ )
				{
					text << buttons[i] << ": ";
					
					if( i < currentButton )
						text << buttonValues[buttons[i]] << "\n";
					else
					{
						text << "?";
						break;
					}
				}
				
				if( currentButton >= buttons.size() )
					text << "\nPress any button to quit.";
			}
			
			Font::get("bitmap")->render( origin, text.str() );
			
			Screen::get()->render();
			
			lastDrawTicks = ticks;
		}
	}
	
	if( controller != NULL )
	{
		// Print to logger
		Logger::get() << "[" << controller->getName() << "]" << Logger::endl;
				
		for( unsigned int i = 0 ; i < buttons.size() ; i++ )
		{
			if( i < currentButton )
				Logger::get() << buttons[i] << "=" << buttonValues[buttons[i]] << Logger::endl;
			else
				break;
		}
	}
	
	Font::destroy();
	Controller::destroy();
	Screen::destroy();
	Logger::destroy();
	
	return 0;
}
