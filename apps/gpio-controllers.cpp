#include <tools/logger/Stdout.h>
#include <tools/gpio/GPIO.h>
#include <data/parser/IniParser.h>

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
using namespace data;

bool running = true;

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
	
	//vector<VirtualDevice *> ...
	//map<int,pair<int,int>> ... pair<index, value>
	
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
		#ifdef DEBUG0
		Logger::get() << "Reading configuration for GPIO controller \"" << *it << "\"..." << Logger::endl;
		#endif
		
		// UpButton
		if( conf->hasKey( "UpButton", *it ) )
		{
			#ifdef DEBUG0
			Logger::get() << "Up Button is GPIO#" << conf->getValue( "UpButton", *it ) << Logger::endl;
			#endif
		}
		#ifdef DEBUG0
		else
			Logger::get() << "Up Button is not defined." << Logger::endl;
		#endif
		
		// LeftButton
		if( conf->hasKey( "LeftButton", *it ) )
		{
			#ifdef DEBUG0
			Logger::get() << "Left Button is GPIO#" << conf->getValue( "LeftButton", *it ) << Logger::endl;
			#endif
		}
		#ifdef DEBUG0
		else
			Logger::get() << "Left Button is not defined." << Logger::endl;
		#endif
		
		// RightButton
		if( conf->hasKey( "RightButton", *it ) )
		{
			#ifdef DEBUG0
			Logger::get() << "Right Button is GPIO#" << conf->getValue( "RightButton", *it ) << Logger::endl;
			#endif
		}
		#ifdef DEBUG0
		else
			Logger::get() << "Right Button is not defined." << Logger::endl;
		#endif
		
		// DownButton
		if( conf->hasKey( "DownButton", *it ) )
		{
			#ifdef DEBUG0
			Logger::get() << "Down Button is GPIO#" << conf->getValue( "DownButton", *it ) << Logger::endl;
			#endif
		}
		#ifdef DEBUG0
		else
			Logger::get() << "Down Button is not defined." << Logger::endl;
		#endif
		
		// UpAxis
		if( conf->hasKey( "UpAxis", *it ) )
		{
			#ifdef DEBUG0
			Logger::get() << "Up Axis is GPIO#" << conf->getValue( "UpAxis", *it ) << Logger::endl;
			#endif
		}
		#ifdef DEBUG0
		else
			Logger::get() << "Up Axis is not defined." << Logger::endl;
		#endif
		
		// LeftAxis
		if( conf->hasKey( "LeftAxis", *it ) )
		{
			#ifdef DEBUG0
			Logger::get() << "Left Axis is GPIO#" << conf->getValue( "LeftAxis", *it ) << Logger::endl;
			#endif
		}
		#ifdef DEBUG0
		else
			Logger::get() << "Left Axis is not defined." << Logger::endl;
		#endif
		
		// RightAxis
		if( conf->hasKey( "RightAxis", *it ) )
		{
			#ifdef DEBUG0
			Logger::get() << "Right Axis is GPIO#" << conf->getValue( "RightAxis", *it ) << Logger::endl;
			#endif
		}
		#ifdef DEBUG0
		else
			Logger::get() << "Right Axis is not defined." << Logger::endl;
		#endif
		
		// DownAxis
		if( conf->hasKey( "DownAxis", *it ) )
		{
			#ifdef DEBUG0
			Logger::get() << "Down Axis is GPIO#" << conf->getValue( "DownAxis", *it ) << Logger::endl;
			#endif
		}
		#ifdef DEBUG0
		else
			Logger::get() << "Down Axis is not defined." << Logger::endl;
		#endif
	}
	
	delete conf;
	
	GPIO::initialize();
	GPIO::get()->open( 21, GPIO::In );
	GPIO::get()->open( 20, GPIO::In );
	GPIO::get()->logStatus();

	bool pushed1 = false;
	bool pushed2 = false;

	while( running )
	{
		bool status1 = GPIO::get()->read( 21 );
		bool status2 = GPIO::get()->read( 20 );
		
		if( status1 != pushed1 )
		{
			pushed1 = status1;
			
			if( !pushed1 )
			{
				Logger::get() << "Button1 pushed." << Logger::endl;
			}
			else
			{
				Logger::get() << "Button1 released." << Logger::endl;
			}
		}
		
		if( status2 != pushed2 )
		{
			pushed2 = status2;
			
			if( !pushed2 )
			{
				Logger::get() << "Button2 pushed." << Logger::endl;
			}
			else
			{
				Logger::get() << "Button2 released." << Logger::endl;
			}
		}
		
		// Wait 100 ms
		usleep( 100000 );
	}
	
	GPIO::destroy();
	Logger::destroy();
	
	return 0;
}

