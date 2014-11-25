#include <tools/logger/Stdout.h>
#include <tools/gpio/GPIO.h>

#include <unistd.h>

using namespace tools::logger;
using namespace tools::gpio;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	bool running = true;
	int times = 50;
	
	GPIO::initialize();
	GPIO::get()->open( 21, GPIO::In );

	bool pushed = false;

	while( running )
	{
		bool status = GPIO::get()->read( 21 );
		
		if( status != pushed )
		{
			pushed = status;
			
			if( pushed )
				Logger::get() << "Button pushed." << Logger::endl;
			else
				Logger::get() << "Button released." << Logger::endl;
		}
		
		// Wait 100 ms
		usleep( 100000 );
		times--;
		
		if( times <= 0 )
			running = false;
	}
	
	GPIO::destroy();
	Logger::destroy();
	
	return 0;
}

