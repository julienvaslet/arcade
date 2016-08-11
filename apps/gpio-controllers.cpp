#include <tools/logger/Stdout.h>
#include <tools/gpio/GPIO.h>
#include <data/parser/IniParser.h>
#include <controller/VirtualDevice.h>

#include <cstdlib>
#include <unistd.h>
#include <signal.h>

#include <map>
#include <vector>
#include <set>
#include <sstream>
#include <fstream>

#define GPIO_CONFIGURATION_FILE	"data/controllers/gpio.conf"

using namespace std;
using namespace tools::logger;
using namespace tools::gpio;
using namespace controller;
using namespace data;

bool running = true;

struct GpioAction {
	VirtualDevice * device;
	int event;
	int eventModifier;
	bool pushed;
};

void signalHandler( int signo )
{
	if( signo == SIGTERM )
	{
		#ifdef DEBUG0
		Logger::get() << "Received SIGTERM." << Logger::endl;
		#endif
		
		running = false;
	}
	else if( signo == SIGINT )
	{
		#ifdef DEBUG0
		Logger::get() << "Received SIGINT." << Logger::endl;
		#endif
		
		running = false;
	}
}

int toInt( const string& value )
{
	int iValue = 0;
	
	istringstream iss( value );
	
	if( iss )
		iss >> iValue;
	
	return iValue;
}

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( signal( SIGTERM, signalHandler ) == SIG_ERR )
	{
		Logger::get() << "Unable to set handle for SIGTERM signals." << Logger::endl;
		Logger::destroy();
		exit( 1 );
	}
	
	if( signal( SIGINT, signalHandler ) == SIG_ERR )
	{
		Logger::get() << "Unable to set handle for SIGKILL signals." << Logger::endl;
		Logger::destroy();
		exit( 1 );
	}
	
	vector<VirtualDevice *> devices;
	map<int, vector<GpioAction>> gpioActions;
	
	ifstream iniFile( GPIO_CONFIGURATION_FILE );

	if( !iniFile.is_open() )
	{
		Logger::get() << "Unable to open file " GPIO_CONFIGURATION_FILE "." << Logger::endl;
		Logger::destroy();
		exit( 2 );
	}

	stringstream ss;
	ss << iniFile.rdbuf();
	iniFile.close();
	
	parser::IniParser * parser = new parser::IniParser( ss.str() );
	ini::Configuration * conf = parser->parse();
	delete parser;
	
	set<string> sections = conf->getSections();
	
	for( set<string>::iterator it = sections.begin() ; it != sections.end() ; it++ )
	{
		VirtualDevice * device = new VirtualDevice( *it );
		devices.push_back( device );
		
		// NorthButton
		if( conf->hasKey( "NorthButton", *it ) )
		{		
			if( device->enableKey( BTN_NORTH ) )
				gpioActions[toInt( conf->getValue( "NorthButton", *it ) )].push_back( { device, BTN_NORTH, 1, false } );
		}
		
		// WestButton
		if( conf->hasKey( "WestButton", *it ) )
		{
			if( device->enableKey( BTN_WEST ) )
				gpioActions[toInt( conf->getValue( "WestButton", *it ) )].push_back(  { device, BTN_WEST, 1, false } );
		}
		
		// EastButton
		if( conf->hasKey( "EastButton", *it ) )
		{
			if( device->enableKey( BTN_EAST ) )
				gpioActions[toInt( conf->getValue( "EastButton", *it ) )].push_back( { device, BTN_EAST, 1, false } );
		}
		
		// SouthButton
		if( conf->hasKey( "SouthButton", *it ) )
		{
			if( device->enableKey( BTN_SOUTH ) )
				gpioActions[toInt( conf->getValue( "SouthButton", *it ) )].push_back( { device, BTN_SOUTH, 1, false } );
		}
		
		// LeftTrigger
		if( conf->hasKey( "LeftTrigger", *it ) )
		{
			if( device->enableKey( BTN_TL ) )
				gpioActions[toInt( conf->getValue( "LeftTrigger", *it ) )].push_back( { device, BTN_TL, 1, false } );
		}
		
		// RightTrigger
		if( conf->hasKey( "RightTrigger", *it ) )
		{
			if( device->enableKey( BTN_TR ) )
				gpioActions[toInt( conf->getValue( "RightTrigger", *it ) )].push_back( { device, BTN_TR, 1, false } );
		}
		
		// SelectButton
		if( conf->hasKey( "SelectButton", *it ) )
		{
			if( device->enableKey( BTN_SELECT ) )
				gpioActions[toInt( conf->getValue( "SelectButton", *it ) )].push_back( { device, BTN_SELECT, 1, false } );
		}
		
		// StartButton
		if( conf->hasKey( "StartButton", *it ) )
		{
			if( device->enableKey( BTN_START ) )
				gpioActions[toInt( conf->getValue( "StartButton", *it ) )].push_back( { device, BTN_START, 1, false } );
		}
		
		// UpAxis
		if( conf->hasKey( "UpAxis", *it ) )
		{
			if( device->enableAbsoluteAxis( ABS_Y ) )
				gpioActions[toInt( conf->getValue( "UpAxis", *it ) )].push_back( { device, ABS_Y, -1, false } );
		}
		
		// DownAxis
		if( conf->hasKey( "DownAxis", *it ) )
		{
			if( device->enableAbsoluteAxis( ABS_Y ) )
				gpioActions[toInt( conf->getValue( "DownAxis", *it ) )].push_back( { device, ABS_Y, 1, false } );
		}
		
		// LeftAxis
		if( conf->hasKey( "LeftAxis", *it ) )
		{
			if( device->enableAbsoluteAxis( ABS_X ) )
				gpioActions[toInt( conf->getValue( "LeftAxis", *it ) )].push_back( { device, ABS_X, -1, false } );
		}
		
		// RightAxis
		if( conf->hasKey( "RightAxis", *it ) )
		{
			if( device->enableAbsoluteAxis( ABS_X ) )
				gpioActions[toInt( conf->getValue( "RightAxis", *it ) )].push_back( { device, ABS_X, 1, false } );
		}
		
		device->create();
	}
	
	delete conf;
	GPIO::initialize();
	
	for( map<int, vector<GpioAction>>::iterator it = gpioActions.begin() ; it != gpioActions.end() ; it++ )
		GPIO::get()->open( it->first, GPIO::In );

	while( running )
	{
		for( map<int, vector<GpioAction>>::iterator it = gpioActions.begin() ; it != gpioActions.end() ; it++ )
		{
			bool status = GPIO::get()->read( it->first );
			
			for( vector<GpioAction>::iterator action = it->second.begin() ; action != it->second.end() ; action++ )
			{
				if( status != action.pushed )
				{
					action.pushed = status;
				
					if( action.pushed )
					{
						#ifdef DEBUG0
						Logger::get() << "GPIO#" << it->first << " pushed." << Logger::endl;
						#endif
					
						// Warning: It may provide two event press & release for axis
						if( action.event != ABS_X && action.event != ABS_Y )
						{
							action.device->pressKey( action.event, false );
						
							#ifdef DEBUG0
							Logger::get() << "Joystick \"" << action.device->getName() << " pressed " << action.event << " button." << Logger::endl;
							#endif
						}
						else
						{
							action.device->sendAbsoluteAxis( action.event, 32767 * action.eventModifier, false );
						
							#ifdef DEBUG0
							Logger::get() << "Joystick \"" << action.device->getName() << " pressed " << action.event << " axis with modifier " << action.eventModifier << "." << Logger::endl;
							#endif
						}
					}
					else
					{
						#ifdef DEBUG0
						Logger::get() << "GPIO#" << it->first << " released." << Logger::endl;
						#endif
					
						// Warning: It may provide two event press & release for axis
						if( action.event != ABS_X && action.event != ABS_Y )
						{
							action.device->releaseKey( action.event );
					
							#ifdef DEBUG0
							Logger::get() << "Joystick \"" << action.device->getName() << " released " << action.event << " button." << Logger::endl;
							#endif
						}
						else
						{
							action.device->sendAbsoluteAxis( action.event, 0, false );
						
							#ifdef DEBUG0
							Logger::get() << "Joystick \"" << action.device->getName() << " pressed " << action.event << " axis with modifier " << action.eventModifier << "." << Logger::endl;
							#endif
						}
					}
				}
			}
		}
		
		for( vector<VirtualDevice *>::iterator it = devices.begin() ; it != devices.end() ; it++ )
			(*it)->flush();
		
		// Wait 25 ms
		usleep( 25000 );
	}
	
	for( vector<VirtualDevice *>::iterator it = devices.begin() ; it != devices.end() ; it++ )
		delete *it;
	
	GPIO::destroy();
	Logger::destroy();
	
	return 0;
}

