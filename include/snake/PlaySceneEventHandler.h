#ifndef __SNAKE_PLAYSCENE_EVENTHANDLER_H
#define __SNAKE_PLAYSCENE_EVENTHANDLER_H	1

#include <controller/EventHandler.h>
#include <snake/PlayScene.h>

using namespace controller;

namespace snake
{
	class PlaySceneEventHandler : public EventHandler
	{
		protected:
			PlayScene * scene;
			unsigned int lastTickEvent;
		
		public:
			PlaySceneEventHandler( PlayScene * scene );
			virtual ~PlaySceneEventHandler();
			virtual void handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp );
	};
}

#endif

