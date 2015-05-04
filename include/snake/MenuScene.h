#ifndef __SNAKE_MENUSCENE_H
#define __SNAKE_MENUSCENE_H	1

#include <game/Scene.h>
#include <opengl/Color.h>
#include <opengl/ColoredRectangle.h>

#include <vector>

using namespace game;
using namespace opengl;
using namespace std;

namespace snake
{
	class MenuScene : public Scene
	{	
		protected:
			unsigned int lastDrawTicks;
			ColoredRectangle * selectIcon;
			unsigned int selectedAction;
			bool selectingMap;
			vector<string> actions;
			vector<string> maps;
			
		public:
			MenuScene();
			virtual ~MenuScene();
			
			virtual void handleEvent( SDL_Event * event );
			virtual void live( unsigned int ticks );
			virtual void render( unsigned int ticks );
			
			void selectLowerAction( int times = 1 );
			void selectUpperAction( int times = 1 );
			void selectCurrentAction();
	};
}

#endif
