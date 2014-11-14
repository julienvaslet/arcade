#ifndef __CONTROLLER_VIRTUALDEVICE_H
#define __CONTROLLER_VIRTUALDEVICE_H	1

#include <string>

#include <linux/input.h>
#include <linux/uinput.h>

using namespace std;

#define UINPUT_DEVICE	"/dev/uinput"

namespace controller
{
	class VirtualDevice
	{
		protected:
			int descriptor;
			uinput_user_dev device;
			input_event event;
		
		public:
			VirtualDevice( const string& name );
			virtual ~VirtualDevice();
			
			const char * getName() const;
			
			bool enableEvents( unsigned short int type );
			bool enableEvent( unsigned short int type, unsigned short int event );
			void sendEvent( unsigned short int type, unsigned short int event, int value );
			
			bool enableSynchronizationEvents();
			bool enableSynchronizationEvent( unsigned short int syn );
			void sendSynchronizationEvent( unsigned short int syn, int value );
			
			bool enableKeyboardEvents();
			bool enableKey( unsigned short int key );
			void sendKey( unsigned short int key, int value );
			
			bool enableRelativeAxisEvents();
			bool enableRelativeAxis( unsigned short int rel );
			void sendRelativeAxis( unsigned short int rel, int value );
			
			bool enableAbsoluteAxisEvents();
			bool enableAbsoluteAxis( unsigned short int abs );
			void sendAbsoluteAxis( unsigned short int abs, int value );

			bool create();
			
			bool isValid() const;
	};
}

#endif
