#ifndef __BLOCKGAME_SCORESCENE_H
#define __BLOCKGAME_SCORESCENE_H	1

#include <game/Scene.h>
#include <opengl/Color.h>

#include <string>

using namespace game;
using namespace opengl;
using namespace std;

namespace blockgame
{
	class ScoreScene : public Scene
	{	
		protected:
			unsigned int lastDrawTicks;
			unsigned int lastAlphaChangeTicks;
			bool increaseAlpha;
			Color textColor;
			
			string score;
			
		public:
			ScoreScene( unsigned int score );
			virtual ~ScoreScene();
			
			virtual void handleEvent( SDL_Event * event );
			virtual void live( unsigned int ticks );
			virtual void render( unsigned int ticks );
	};
}

#endif
