#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <tools/logger/Stdout.h>

#include <opengl/Screen.h>
#include <opengl/OpenGL.h>
#include <opengl/Camera.h>
#include <opengl/BitmapFont.h>

#include <blockgame/Grid.h>
#include <blockgame/Piece.h>

using namespace opengl;
using namespace std;
using namespace tools::logger;
using namespace blockgame;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( !Screen::initialize( "Blockgame" ) )
	{
		Logger::get() << "Unable to initialize screen. Exiting.\n";
		return 1;
	}
	
	bool running = true;
	SDL_Event lastEvent;
	unsigned int lastDrawTicks = 0;
	unsigned int lastBlockMove = 0;
	unsigned int level = 1;
	unsigned int lines = 0;
	unsigned int score = 0;
	
	// Integer string variables
	stringstream levelStr( "1" );
	stringstream linesStr( "0" );
	stringstream scoreStr( "0" );
	
	// Blocks' vectors
	Grid * blocks = new Grid( 10.0f, 20.0f );
	Grid * background = new Grid( 10.0f, 20.0f );
	Piece * fallingBlock = Piece::generate();
	Piece * nextBlock = Piece::generate();
	
	Screen::get()->setClearColor( Color( 0.0f, 0.0f, 0.0f, 0.0f ) );
	new BitmapFont( "data/fonts/bitmap.bmp", 32, 32, 7, 1 );

	Camera camera;
	camera.getEye().moveTo( 0.0f, 0.0f, 70.0f );
	camera.getCenter().moveTo( 0.0f, 0.0f, 0.0f );
	
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	// Fill background vector
	Point2D position( 0.0f, 0.0f );
	Color backgroundColor( 0.0f, 0.1f, 0.1f );
	
	for( unsigned int y = 0 ; y < 20 ; y++ )
	{
		for( unsigned int x = 0 ; x < 10 ; x++ )
		{
			position.moveTo( x, y );
			background->insert( new Block( position, backgroundColor ) );
		}
	}
	
	lastBlockMove = SDL_GetTicks();
	
	while( running )
	{
		while( SDL_PollEvent( &lastEvent ) )
		{
			switch( lastEvent.type )
			{
				case SDL_QUIT:
				{
					running = false;
					break;
				}
				
				case SDL_KEYDOWN:
				{
		            if( lastEvent.key.keysym.sym == SDLK_ESCAPE )
						running = false;
						
					else if( lastEvent.key.keysym.sym == SDLK_LEFT )
					{
						// Move the falling block on the left
						if( fallingBlock != NULL )
						{
							fallingBlock->moveBy( -1.0f, 0.0f );
							
							if( fallingBlock->isInCollision( blocks ) )
								fallingBlock->moveBy( 1.0f, 0.0f );
							else
								fallingBlock->correctPosition( blocks->getWidth(), blocks->getHeight() );
						}
					}
					else if( lastEvent.key.keysym.sym == SDLK_RIGHT )
					{
						// Move the falling block on the right
						if( fallingBlock != NULL )
						{
							fallingBlock->moveBy( 1.0f, 0.0f );
							
							if( fallingBlock->isInCollision( blocks ) )
								fallingBlock->moveBy( -1.0f, 0.0f );
							else
								fallingBlock->correctPosition( blocks->getWidth(), blocks->getHeight() );
						}
					}
					else if( lastEvent.key.keysym.sym == SDLK_UP )
					{
						// Rotate the falling block
						fallingBlock->rotate();
						// TODO: Should test blocks collision
						fallingBlock->correctPosition( blocks->getWidth(), blocks->getHeight() );
					}
					else if( lastEvent.key.keysym.sym == SDLK_DOWN )
					{
						// Drop the falling block
						score += ( 21 + ( 3 * level ) - ( blocks->getHeight() - fallingBlock->getY() ) );
						
						while( !fallingBlock->isAtGround() && !fallingBlock->isInCollision( blocks ) )
							fallingBlock->moveBy( 0.0f, -1.0f );
						
						fallingBlock->moveBy( 0.0f, 1.0f );
						blocks->insert( fallingBlock );
						delete fallingBlock;
						fallingBlock = nextBlock;
						nextBlock = Piece::generate();
					
						lines += blocks->deleteFullLines();
						level = ( lines / 10 ) + 1;
						
						linesStr.str("");
						linesStr << lines;
						
						levelStr.str("");
						levelStr << level;
						
						scoreStr.str("");
						scoreStr << score;
					
						if( fallingBlock->isInCollision( blocks ) )
							running = false;
					}

					break;
				}
			}
		}
		
		unsigned int ticks = SDL_GetTicks();
		
		if( fallingBlock != NULL )
		{
			if( ticks - lastBlockMove > static_cast<unsigned int>( 500.0f - ((static_cast<float>( level ) - 1.0f) * 50.0f)) )
			{
				// Free-fall action
				fallingBlock->moveBy( 0.0f, -1.0f );
				
				if( fallingBlock->isAtGround() || fallingBlock->isInCollision( blocks ) )
				{					
					fallingBlock->moveBy( 0.0f, 1.0f );
					score += ( 21 + ( 3 * level ) - ( blocks->getHeight() - fallingBlock->getY() ) );
					
					blocks->insert( fallingBlock );
					delete fallingBlock;
					fallingBlock = nextBlock;
					nextBlock = Piece::generate();
					
					lines += blocks->deleteFullLines();
					level = ( lines / 10 ) + 1;
					
					linesStr.str("");
					linesStr << lines;
					
					levelStr.str("");
					levelStr << level;
					
					scoreStr.str("");
					scoreStr << score;
			
					if( fallingBlock->isInCollision( blocks ) )
						running = false;
				}
				
				lastBlockMove = ticks;
			}
		}
		
		if( ticks - lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			
			float screenRatio = static_cast<float>( Screen::get()->getWidth() ) / static_cast<float>( Screen::get()->getHeight() );
			float screenSize = 35.0f;
			camera.setOrthogonal( -1.0f * screenSize * screenRatio, screenSize * screenRatio, -1.0f * screenSize, screenSize, 1.0f, 100.0f );
			
			glMatrixMode( GL_MODELVIEW );
			camera.look();
			
			glMultMatrixf( Matrix::translation( -28.0f, -30.0f, 0.0f ).get() );
			background->render();
			blocks->render();
			
			if( fallingBlock != NULL )
				fallingBlock->render();
			
			// User Interface
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 80 ), "Score", 1.0f );
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 112 ), scoreStr.str(), 1.0f );
			
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 160 ), "Lines", 1.0f );
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 192 ), linesStr.str(), 1.0f );
			
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 240 ), "Level", 1.0f );
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 272 ), levelStr.str(), 1.0f );
			
			Screen::get()->render();
			
			lastDrawTicks = ticks;
		}
	}
	
	delete blocks;
	delete background;
	
	if( fallingBlock != NULL )
		delete fallingBlock;
		
	if( nextBlock != NULL )
		delete nextBlock;

	Font::destroy();
	Screen::destroy();
	Logger::destroy();
	
	return 0;
}

