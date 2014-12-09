#include <tools/logger/Stdout.h>
#include <tools/gpio/GPIO.h>
#include <data/parser/IniParser.h>
#include <controller/VirtualDevice.h>

#include <cstdlib>
#include <unistd.h>
#include <signal.h>

#include <map>
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
	map<int, GpioAction> gpioActions;
	
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
				gpioActions[toInt( conf->getValue( "NorthButton", *it ) )] = { device, BTN_NORTH, 1, false };
		}
		
		// WestButton
		if( conf->hasKey( "WestButton", *it ) )
		{
			if( device->enableKey( BTN_WEST ) )
				gpioActions[toInt( conf->getValue( "WestButton", *it ) )] = { device, BTN_WEST, 1, false };
		}
		
		// EastButton
		if( conf->hasKey( "EastButton", *it ) )
		{
			if( device->enableKey( BTN_EAST ) )
				gpioActions[toInt( conf->getValue( "EastButton", *it ) )] = { device, BTN_EAST, 1, false };
		}
		
		// SouthButton
		if( conf->hasKey( "SouthButton", *it ) )
		{
			if( device->enableKey( BTN_SOUTH ) )
				gpioActions[toInt( conf->getValue( "SouthButton", *it ) )] = { device, BTN_SOUTH, 1, false };
		}
		
		// UpAxis
		if( conf->hasKey( "UpAxis", *it ) )
		{
			if( device->enableAbsoluteAxis( ABS_Y ) )
				gpioActions[toInt( conf->getValue( "UpAxis", *it ) )] = { device, ABS_Y, 1, false };
		}
		
		// DownAxis
		if( conf->hasKey( "DownAxis", *it ) )
		{
			if( device->enableAbsoluteAxis( ABS_Y ) )
				gpioActions[toInt( conf->getValue( "DownAxis", *it ) )] = { device, ABS_Y, 1, false };
		}
		
		// LeftAxis
		if( conf->hasKey( "LeftAxis", *it ) )
		{
			if( device->enableAbsoluteAxis( ABS_X ) )
				gpioActions[toInt( conf->getValue( "LeftAxis", *it ) )] = { device, ABS_X, 1, false };
		}
		
		// RightAxis
		if( conf->hasKey( "RightAxis", *it ) )
		{
			if( device->enableAbsoluteAxis( ABS_X ) )
				gpioActions[toInt( conf->getValue( "RightAxis", *it ) )] = { device, ABS_X, 1, false };
		}
		
		device->create();
	}
	
	delete conf;
	GPIO::initialize();
	
	for( map<int, GpioAction>::iterator it = gpioActions.begin() ; it != gpioActions.end() ; it++ )
		GPIO::get()->open( it->first, GPIO::In );

	while( running )
	{
		for( map<int, GpioAction>::iterator it = gpioActions.begin() ; it != gpioActions.end() ; it++ )
		{
			bool status = GPIO::get()->read( it->first );
			
			if( status != it->second.pushed )
			{
				it->second.pushed = status;
				
				if( it->second.pushed )
				{
					#ifdef DEBUG0
					Logger::get() << "GPIO#" << it->first << " pushed." << Logger::endl;
					#endif
					
					// Warning: It may provide two event press & release for axis
					if( it->second.event != ABS_X && it->second.event != ABS_Y )
					{
						it->second.device->pressKey( it->second.event, false );
						
						#ifdef DEBUG0
						Logger::get() << "Joystick \"" << it->second.device->getName() << " pressed " << it->second.event << " button." << Logger::endl;
						#endif
					}
					else
					{
						it->second.device->sendAbsoluteAxis( it->second.event, 32767 * it->second.eventModifier, false );
						
						#ifdef DEBUG0
						Logger::get() << "Joystick \"" << it->second.device->getName() << " pressed " << it->second.event << " axis with modifier " << it->second.eventModifier << "." << Logger::endl;
						#endif
					}
				}
				else
				{
					#ifdef DEBUG0
					Logger::get() << "GPIO#" << it->first << " released." << Logger::endl;
					#endif
					
					// Warning: It may provide two event press & release for axis
					if( it->second.event != ABS_X && it->second.event != ABS_Y )
					{
						it->second.device->releaseKey( it->second.event );
					
						#ifdef DEBUG0
						Logger::get() << "Joystick \"" << it->second.device->getName() << " released " << it->second.event << " button." << Logger::endl;
						#endif
					}
					else
					{
						it->second.device->sendAbsoluteAxis( it->second.event, 0, false );
						
						#ifdef DEBUG0
						Logger::get() << "Joystick \"" << it->second.device->getName() << " pressed " << it->second.event << " axis with modifier " << it->second.eventModifier << "." << Logger::endl;
						#endif
					}
				}
			}
		}
		
		for( vector<VirtualDevice *>::iterator it = devices.begin() ; it != devices.end() ; it++ )
			(*it)->flush();
		
		// Wait 100 ms
		usleep( 100000 );
	}
	
	for( vector<VirtualDevice *>::iterator it = devices.begin() ; it != devices.end() ; it++ )
		delete *it;
	
	GPIO::destroy();
	Logger::destroy();
	
	return 0;
}

