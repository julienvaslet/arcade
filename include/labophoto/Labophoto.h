#ifndef __LABOPHOTO_LABOPHOTO_H
#define __LABOPHOTO_LABOPHOTO_H	1

#include <SDL2/SDL.h>
#include <opengl/ColoredRectangle.h>
#include <opengl/TexturedRectangle.h>
#include <opengl/ui/UserInterface.h>
#include <tools/camera/GphotoCamera.h>

using namespace tools::camera;
using namespace opengl;
using namespace opengl::ui;

namespace labophoto
{
	class Labophoto
	{
		protected:
			static Labophoto * instance;
			GphotoCamera * camera;
			UserInterface * ui;
			TexturedRectangle * image;
			ColoredRectangle * workspace;
			
			void loadModeSelectionUI();
			void loadCameraSelectionUI();
			void loadCameraConfigurationUI();
			
			void reloadAvailableCameras();
			void reloadCameraConfiguration();
			
			void setMode( short int mode );
			void selectCamera( const string& port );
			void captureImage();
			void setCameraShutterSpeed( const string& value );
			void setCameraAperture( const string& value );
			void setCameraIso( const string& value );
			void setCameraWhiteBalance( const string& value );
		
		public:
			Labophoto();
			~Labophoto();
			
			bool isRunning();
			void render( unsigned int ticks );
			void handleEvent( SDL_Event * event );
			
			void resizeWorkspace();
			
			static Labophoto * get();
			
			static bool reloadCameraListEvent( Element * element, const event::Event * event );
			static bool currentModeChangedEvent( Element * element, const event::Event * event );
			static bool cameraChangedEvent( Element * element, const event::Event * event );
			static bool takePreviewEvent( Element * element, const event::Event * event );
			static bool changeShutterSpeedEvent( Element * element, const event::Event * event );
			static bool changeApertureEvent( Element * element, const event::Event * event );
			static bool changeIsoEvent( Element * element, const event::Event * event );
			static bool changeWhiteBalanceEvent( Element * element, const event::Event * event );
	};
}

#endif
