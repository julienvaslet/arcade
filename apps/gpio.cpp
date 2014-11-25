#include <tools/logger/Stdout.h>

using namespace tools::logger;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	bool running = true;

	while( running )
	{
		break;
	}
	
	Logger::destroy();
	
	return 0;
}

