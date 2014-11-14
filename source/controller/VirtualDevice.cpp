#include <controller/VirtualDevice.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

extern int errno;

namespace controller
{
	VirtualDevice::VirtualDevice( const string& name ) : descriptor(-1)
	{
		memset( &(this->device), 0, sizeof(this->device) );
		
		snprintf( this->device.name, UINPUT_MAX_NAME_SIZE, name.c_str() );
		this->device.id.bustype = BUS_USB;
		this->device.id.vendor  = 0x1;
		this->device.id.product = 0x1;
		this->device.id.version = 2;
		
		this->descriptor = open( UINPUT_DEVICE, O_WRONLY | O_NONBLOCK );
		
		#ifdef DEBUG0
		if( this->descriptor == -1 )
			Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to open uinput device (" << UINPUT_DEVICE << "): " << strerror( errno ) << Logger::endl;
		#endif
	}
	
	VirtualDevice::~VirtualDevice()
	{
		if( this->descriptor > 0 )
		{
			ioctl( this->descriptor, UI_DEV_DESTROY );
			close( this->descriptor );
		}
	}

	const char * VirtualDevice::getName() const
	{
		return this->device.name;
	}
	
	// Global event functions
	
	bool VirtualDevice::enableEvents( unsigned short int type )
	{	
		return ( ioctl( this->descriptor, UI_SET_EVBIT, type ) == 0 );
	}
	
	bool VirtualDevice::enableEvent( unsigned short int type, unsigned short int event )
	{
		return ( ioctl( this->descriptor, type, event ) == 0 );
	}
		
	void VirtualDevice::sendEvent( unsigned short int type, unsigned short int event, int value )
	{
		if( this->descriptor > 0 )
		{
			memset( &(this->event), 0, sizeof(this->event) );
			gettimeofday( &(this->event.time), NULL );
			
			this->event.type = type;
			this->event.code = event;
			this->event.value = value;
			
			write( this->descriptor, &(this->event), sizeof(this->event) );
		}
	}
	
	// Synchronization events
	
	bool VirtualDevice::enableSynchronizationEvents()
	{
		bool status = true;
		
		if( !this->enableEvents( EV_SYN ) )
		{
			status = false;
			
			#ifdef DEBUG0
			Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to enable synchronization events: " << strerror( errno ) << Logger::endl;
			#endif
		}
		
		return status;
	}
	
	bool VirtualDevice::enableSynchronizationEvent( unsigned short int syn )
	{
		bool status = true;
		
		if( !this->enableEvent( UI_SET_EVBIT, syn ) )
		{
			status = false;
			
			#ifdef DEBUG0
			Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to enable the specified synchronization event: " << strerror( errno ) << Logger::endl;
			#endif
		}
		
		return status;
	}
	
	void VirtualDevice::sendSynchronizationEvent( unsigned short int syn, int value )
	{
		this->sendEvent( EV_SYN, syn, value );
	}
	
	// Keyboard events
	
	bool VirtualDevice::enableKeyboardEvents()
	{
		bool status = true;
		
		if( !this->enableEvents( EV_KEY ) )
		{
			status = false;
			
			#ifdef DEBUG0
			Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to enable keyboard events: " << strerror( errno ) << Logger::endl;
			#endif
		}
		
		return status;
	}
	
	bool VirtualDevice::enableKey( unsigned short int key )
	{
		bool status = true;
		
		if( !this->enableEvent( UI_SET_KEYBIT, key ) )
		{
			status = false;
			
			#ifdef DEBUG0
			Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to enable the specified keyboard event: " << strerror( errno ) << Logger::endl;
			#endif
		}
		
		return status;
	}
	
	void VirtualDevice::sendKey( unsigned short int key, int value )
	{
		this->sendEvent( EV_KEY, key, value );
	}
	
	// Relative Axis events
	
	bool VirtualDevice::enableRelativeAxisEvents()
	{
		bool status = true;
		
		if( !this->enableEvents( EV_REL ) )
		{
			status = false;
			
			#ifdef DEBUG0
			Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to enable relative axis events: " << strerror( errno ) << Logger::endl;
			#endif
		}
		
		return status;
	}
	
	bool VirtualDevice::enableRelativeAxis( unsigned short int rel )
	{
		bool status = true;
		
		if( !this->enableEvent( UI_SET_RELBIT, rel ) )
		{
			status = false;
			
			#ifdef DEBUG0
			Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to enable the specified relative axis event: " << strerror( errno ) << Logger::endl;
			#endif
		}
		
		return status;
	}
	
	void VirtualDevice::sendRelativeAxis( unsigned short int rel, int value )
	{
		this->sendEvent( EV_REL, rel, value );
	}
	
	// Absolute Axis events
	
	bool VirtualDevice::enableAbsoluteAxisEvents()
	{
		bool status = true;
		
		if( !this->enableEvents( EV_ABS ) )
		{
			status = false;
			
			#ifdef DEBUG0
			Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to enable absolute axis events: " << strerror( errno ) << Logger::endl;
			#endif
		}
		
		return status;
	}
	
	bool VirtualDevice::enableAbsoluteAxis( unsigned short int abs )
	{
		bool status = true;
		
		if( !this->enableEvent( UI_SET_ABSBIT, abs ) )
		{
			status = false;
			
			#ifdef DEBUG0
			Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to enable the specified absolute axis event: " << strerror( errno ) << Logger::endl;
			#endif
		}
		
		return status;
	}
	
	void VirtualDevice::sendAbsoluteAxis( unsigned short int abs, int value )
	{
		this->sendEvent( EV_ABS, abs, value );
	}
	
	bool VirtualDevice::create()
	{
		if( this->descriptor > 0 )
		{
			write( this->descriptor, &device, sizeof(device) );
		
			// Create the virtual device
			if( ioctl( this->descriptor, UI_DEV_CREATE ) == -1 )
			{
				#ifdef DEBUG0
				Logger::get() << "[VirtualDevice#" << this->getName() << "] Unable to create the device: " << strerror( errno ) << Logger::endl;
				#endif
				
				close( this->descriptor );
				this->descriptor = -1;
			}
			#ifdef DEBUG0
			else
				Logger::get() << "[VirtualDevice#" << this->getName() << "] Created." << Logger::endl;
			#endif
		}
		
		return false;
	}
	
	bool VirtualDevice::isValid() const
	{
		return this->descriptor > 0;
	}
}

