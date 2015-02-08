#ifndef __BLOCKGAME_PLAYSCENE_H
#define __BLOCKGAME_PLAYSCENE_H	1

#include <game/Scene.h>
#include <blockgame/Grid.h>
#include <blockgame/Piece.h>

#include <sstream>
#include <string>

using namespace game;
using namespace std;

namespace blockgame
{
	class PlayScene : public Scene
	{	
		protected:
			// Render & life ticks
			unsigned int lastDrawTicks;
			unsigned int lastBlockMove;
			unsigned int lastTickEvent;
			
			// Game rules
			unsigned int level;
			unsigned int lines;
			unsigned int score;

			// Strings to print
			stringstream levelStr;
			stringstream linesStr;
			stringstream scoreStr;
	
			// Blocks
			Grid * blocks;
			Grid * background;
			Piece * fallingBlock;
			Piece * nextBlock;
			
			void updateScore();
			void updateLabels();
			void switchBlocks();
			
		public:
			PlayScene();
			virtual ~PlayScene();
			
			virtual void handleEvent( SDL_Event * event );
			virtual void live( unsigned int ticks );
			virtual void render( unsigned int ticks );
			
			void moveLeft();
			void moveRight();
			void moveDown();
			void rotate( bool clockwise );
	};
}

#endif
