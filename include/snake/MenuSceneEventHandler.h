#ifndef __SNAKE_MENUSCENE_EVENTHANDLER_H
#define __SNAKE_MENUSCENE_EVENTHANDLER_H	1

#include <controller/EventHandler.h>
#include <snake/MenuScene.h>

using namespace controller;

namespace snake
{
	class MenuSceneEventHandler : public EventHandler
	{
		protected:
			MenuScene * scene;
			unsigned int lastTickEvent;
		
		public:
			MenuSceneEventHandler( MenuScene * scene );
			virtual ~MenuSceneEventHandler();
			virtual void handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp );
	};
}

#endif

