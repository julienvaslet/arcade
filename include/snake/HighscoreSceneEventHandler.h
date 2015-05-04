#ifndef __SNAKE_HIGHSCORESCENE_EVENTHANDLER_H
#define __SNAKE_HIGHSCORESCENE_EVENTHANDLER_H	1

#include <controller/EventHandler.h>
#include <snake/HighscoreScene.h>

using namespace controller;

namespace snake
{
	class HighscoreSceneEventHandler : public EventHandler
	{
		protected:
			HighscoreScene * scene;
			unsigned int lastTickEvent;
		
		public:
			HighscoreSceneEventHandler( HighscoreScene * scene );
			virtual ~HighscoreSceneEventHandler();
			virtual void handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp );
	};
}

#endif

