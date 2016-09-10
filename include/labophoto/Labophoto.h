#ifndef __LABOPHOTO_LABOPHOTO_H
#define __LABOPHOTO_LABOPHOTO_H	1

#include <SDL2/SDL.h>
#include <opengl/ui/UserInterface.h>
#include <tools/camera/GphotoCamera.h>

using namespace tools::camera;
using namespace opengl::ui;

namespace labophoto
{
	class Labophoto
	{
		protected:
			static Labophoto * instance;
			GphotoCamera * camera;
			UserInterface * ui;
			
			void loadModeSelectionUI();
			void loadCameraSelectionUI();
			void loadCameraConfigurationUI();
			
			void reloadAvailableCameras();
			
			void setMode( short int mode );
			void selectCamera( const string& port );
		
		public:
			Labophoto();
			~Labophoto();
			
			bool isRunning();
			void render( unsigned int ticks );
			void handleEvent( SDL_Event * event );
			
			static Labophoto * get();
			
			static bool reloadCameraListEvent( Element * element, const event::Event * event );
			static bool currentModeChangedEvent( Element * element, const event::Event * event );
			static bool cameraChangedEvent( Element * element, const event::Event * event );
	};
}

#endif
