#ifndef __SNAKE_HIGHSCORESCENE_H
#define __SNAKE_HIGHSCORESCENE_H	1

#include <game/Scene.h>
#include <opengl/ColoredRectangle.h>

#include <vector>
#include <map>
#include <string>

using namespace game;
using namespace opengl;
using namespace std;

namespace snake
{
	class HighscoreScene : public Scene
	{	
		protected:
			unsigned int lastDrawTicks;
			unsigned int lastBlinkTicks;
			unsigned int score;
			bool newHighscore;
			bool shownLetter;
			char letter0;
			char letter1;
			char letter2;
			
			string map;
			vector< pair<string, unsigned int> > highscores;
			
			void loadHighscores();
			void saveHighscores();
			void delayBlinking();
			
		public:
			HighscoreScene( const string& map, unsigned int score );
			virtual ~HighscoreScene();
			
			virtual void handleEvent( SDL_Event * event );
			virtual void live( unsigned int ticks );
			virtual void render( unsigned int ticks );
			
			void upAction( unsigned int times = 1 );
			void downAction( unsigned int times = 1 );
			void buttonAction();
	};
}

#endif
