#ifndef __PONG_PLAYSCENE_H
#define __PONG_PLAYSCENE_H	1

#include <game/Scene.h>
#include <opengl/ColoredRectangle.h>
#include <opengl/Vector.h>
#include <pong/Background.h>

#include <string>

using namespace game;
using namespace opengl;
using namespace std;

namespace pong
{
	class PlayScene : public Scene
	{	
		protected:
			unsigned int lastDrawTicks;
			unsigned int lastBallMove;
			Background * background;
			ColoredRectangle * ball;
			Vector ballDirection;
			ColoredRectangle * racket1;
			ColoredRectangle * racket2;
			unsigned short int scorePlayer1;
			unsigned short int scorePlayer2;
			string sScorePlayer1;
			string sScorePlayer2;
			bool playing;
			bool paused;
			bool player1Ready;
			bool player2Ready;
			
			#ifdef DEBUG1
			unsigned int lastFPSRefresh;
			unsigned int framesPerSecond;
			string sFramesPerSecond;
			unsigned int lastOPSRefresh;
			unsigned int operationsPerSecond;
			string sOperationsPerSecond;
			#endif
			
			void updateScoreStrings();
			void initializeNewGame();
			void initializeBall( unsigned int player = 1 );
			
		public:
			PlayScene();
			virtual ~PlayScene();
			
			virtual void handleEvent( SDL_Event * event );
			virtual void live( unsigned int ticks );
			virtual void render( unsigned int ticks );
			
			void moveUp( unsigned int player, double times = 1.0 );
			void moveDown( unsigned int player, double times = 1.0 );
			void buttonAction( unsigned int player );
			
			void endScene();
	};
}

#endif
