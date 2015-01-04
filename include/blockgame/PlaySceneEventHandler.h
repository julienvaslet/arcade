#ifndef __BLOCKGAME_PLAY_SCENE_EVENT_HANDLER_H
#define __BLOCKGAME_PLAY_SCENE_EVENT_HANDLER_H	1

#include <controller/EventHandler.h>
#include <blockgame/PlayScene.h>

using namespace controller;

namespace blockgame
{
	class PlaySceneEventHandler : public EventHandler
	{
		protected:
			PlayScene * scene;
		
		public:
			PlaySceneEventHandler( PlayScene * scene );
			virtual ~PlaySceneEventHandler();
			virtual void handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp );
	};
}

#endif

