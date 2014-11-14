#include <controller/VirtualDevice.h>
#include <tools/logger/Stdout.h>

#include <unistd.h>

using namespace controller;
using namespace tools::logger;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	VirtualDevice * device = new VirtualDevice( "mydevice0" );
	
	if( device->enableKeyboardEvents() )
	{
		device->enableKey( KEY_D );
	}
	
	device->create();
	sleep( 2 );
	
	device->sendKey( KEY_A, 1 );
	device->sendKey( KEY_A, 0 );
	
	delete device;

	#ifdef DEBUG0	
	Logger::destroy();
	#endif
	
	return 0;
}

