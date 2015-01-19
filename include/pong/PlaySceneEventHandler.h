#ifndef __PONG_PLAYSCENE_EVENTHANDLER_H
#define __PONG_PLAYSCENE_EVENTHANDLER_H	1

#include <controller/EventHandler.h>
#include <pong/PlayScene.h>

using namespace controller;

namespace pong
{
	class PlaySceneEventHandler : public EventHandler
	{
		protected:
			PlayScene * scene;
			unsigned int player;
		
		public:
			PlaySceneEventHandler( PlayScene * scene, unsigned int player );
			virtual ~PlaySceneEventHandler();
			virtual void handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp );
	};
}

#endif

