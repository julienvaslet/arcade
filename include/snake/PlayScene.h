#ifndef __SNAKE_PLAYSCENE_H
#define __SNAKE_PLAYSCENE_H	1

#include <game/Scene.h>
#include <opengl/ColoredRectangle.h>
#include <snake/Snake.h>
#include <snake/Map.h>

#include <string>

using namespace game;
using namespace opengl;
using namespace std;

namespace snake
{
	class PlayScene : public Scene
	{	
		protected:
			unsigned int lastDrawTicks;
			unsigned int lastMoveTicks;
			unsigned int score;
			unsigned int speed;
			bool paused;
			bool growUp;
			
			Map * map;
			Snake * snake;
			ColoredRectangle * apple;
			string sScore;
			
			void updateStrings();
			void generateApple();
			
		public:
			PlayScene( const string& map );
			virtual ~PlayScene();
			
			virtual void handleEvent( SDL_Event * event );
			virtual void live( unsigned int ticks );
			virtual void render( unsigned int ticks );
			
			void moveUp();
			void moveDown();
			void moveLeft();
			void moveRight();
			
			void gameOver();
			void abortGame();
	};
}

#endif
