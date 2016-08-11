#include <tools/logger/Stdout.h>
#include <tools/gpio/GPIO.h>

#include <cstdlib>
#include <unistd.h>
#include <signal.h>

#include <vector>

using namespace std;
using namespace tools::logger;
using namespace tools::gpio;

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
	
	vector<int> ports;
	port.push_back( 2 );
	port.push_back( 3 );
	port.push_back( 4 );
	port.push_back( 17 );
	port.push_back( 27 );
	port.push_back( 22 );
	port.push_back( 10 );
	port.push_back( 9 );
	port.push_back( 11 );
	port.push_back( 5 );
	port.push_back( 6 );
	port.push_back( 13 );
	port.push_back( 19 );
	port.push_back( 26 );
	port.push_back( 14 );
	port.push_back( 15 );
	port.push_back( 18 );
	port.push_back( 23 );
	port.push_back( 24 );
	port.push_back( 25 );
	port.push_back( 8 );
	port.push_back( 7 );
	port.push_back( 12 );
	port.push_back( 16 );
	port.push_back( 20 );
	port.push_back( 21 );

	GPIO::initialize();
	
	for( vector<int>::iterator it = ports.begin() ; it != ports.end() ; it++ )
		GPIO::get()->open( *it, GPIO::In );

	while( running )
	{
		for( vector<int>::iterator it = ports.begin() ; it != ports.end() ; it++ )
		{
			bool status = GPIO::get()->read( *it );
			
			if( status )
			{
				Logger::get() << "GPIO#" << *it << " pushed." << Logger::endl;
			}
		}
		
		// Wait 50 ms
		usleep( 50000 );
	}
	
	GPIO::destroy();
	Logger::destroy();
	
	return 0;
}

