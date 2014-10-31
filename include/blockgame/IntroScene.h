#ifndef __BLOCKGAME_INTROSCENE_H
#define __BLOCKGAME_INTROSCENE_H	1

#include <game/Scene.h>
#include <opengl/Color.h>

using namespace game;
using namespace opengl;

namespace blockgame
{
	class IntroScene : public Scene
	{	
		protected:
			unsigned int lastDrawTicks;
			unsigned int lastAlphaChangeTicks;
			bool increaseAlpha;
			Color textColor;
			
		public:
			IntroScene();
			virtual ~IntroScene();
			
			virtual void handleEvent( SDL_Event * event );
			virtual void live( unsigned int ticks );
			virtual void render( unsigned int ticks );
	};
}

#endif
