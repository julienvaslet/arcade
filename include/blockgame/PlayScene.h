#ifndef __BLOCKGAME_PLAYSCENE_H
#define __BLOCKGAME_PLAYSCENE_H	1

#include <game/Scene.h>
#include <opengl/Camera.h>
#include <blockgame/Grid.h>
#include <blockgame/Piece.h>

#include <sstream>
#include <string>

using namespace game;

namespace blockgame
{
	class PlayScene : public Scene
	{	
		protected:
			// Render & life ticks
			unsigned int lastDrawTicks;
			unsigned int lastBlockMove;
			
			// Game rules
			unsigned int level;
			unsigned int lines;
			unsigned int score;

			// Stringss to print
			stringstream levelStr;
			stringstream linesStr;
			stringstream scoreStr;
	
			// Blocks
			Grid * blocks;
			Grid * background;
			Piece * fallingBlock;
			Piece * nextBlock;
			
			// Camera
			Camera camera;
			
			void updateScore();
			void updateLabels();
			void switchBlocks();
			
		public:
			PlayScene();
			virtual ~PlayScene();
			
			virtual void handleEvent( SDL_Event * event );
			virtual void live( unsigned int ticks );
			virtual void render( unsigned int ticks );
	};
}

#endif
