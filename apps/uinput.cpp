#include <controller/VirtualDevice.h>
#include <tools/logger/Stdout.h>

#include <unistd.h>


//temp
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

extern int errno;

using namespace controller;
using namespace tools::logger;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	VirtualDevice * device = new VirtualDevice( "mydevice0" );
	
	device->enableKey( KEY_E );
	device->enableKey( KEY_H );
	device->enableKey( KEY_L );
	device->enableKey( KEY_O );
	
	device->create();
	sleep( 1 );
	
	device->pressKey( KEY_H );
	device->releaseKey( KEY_H );
	device->pressKey( KEY_E );
	device->releaseKey( KEY_E );
	device->pressKey( KEY_L );
	device->releaseKey( KEY_L );
	device->pressKey( KEY_L );
	device->releaseKey( KEY_L );
	device->pressKey( KEY_O );
	device->releaseKey( KEY_O );
	
	delete device;

	#ifdef DEBUG0	
	Logger::destroy();
	#endif
	
	return 0;
}

