#ifndef __BLOCKGAME_SCORE_SCENE_EVENT_HANDLER_H
#define __BLOCKGAME_SCORE_SCENE_EVENT_HANDLER_H	1

#include <controller/EventHandler.h>
#include <blockgame/ScoreScene.h>

using namespace controller;

namespace blockgame
{
	class ScoreSceneEventHandler : public EventHandler
	{
		protected:
			ScoreScene * scene;
		
		public:
			ScoreSceneEventHandler( ScoreScene * scene );
			virtual ~ScoreSceneEventHandler();
			virtual void handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp );
	};
}

#endif

