#ifndef __BLOCKGAME_INTRO_SCENE_EVENT_HANDLER_H
#define __BLOCKGAME_INTRO_SCENE_EVENT_HANDLER_H	1

#include <controller/EventHandler.h>
#include <blockgame/IntroScene.h>

using namespace controller;

namespace blockgame
{
	class IntroSceneEventHandler : public EventHandler
	{
		protected:
			IntroScene * scene;
		
		public:
			IntroSceneEventHandler( IntroScene * scene );
			virtual ~IntroSceneEventHandler();
			virtual void handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp );
	};
}

#endif

