#ifndef __OVERSCAN_CONFIGSCENE_EVENTHANDLER_H
#define __OVERSCAN_CONFIGSCENE_EVENTHANDLER_H	1

#include <controller/EventHandler.h>
#include <overscan/ConfigScene.h>

using namespace controller;

namespace overscan
{
	class ConfigSceneEventHandler : public EventHandler
	{
		protected:
			ConfigScene * scene;
		
		public:
			ConfigSceneEventHandler( ConfigScene * scene );
			virtual ~ConfigSceneEventHandler();
			virtual void handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp );
	};
}

#endif

