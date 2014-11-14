#include <blockgame/PlayScene.h>
#include <blockgame/ScoreScene.h>

#include <opengl/OpenGL.h>
#include <opengl/Font.h>
#include <opengl/Screen.h>
#include <opengl/Point2D.h>
#include <opengl/Color.h>

#include <audio/Mixer.h>
#include <audio/Song.h>
#include <audio/instrument/Sine.h>
#include <audio/instrument/Silence.h>

#include <cstdlib>
#include <ctime>

using namespace opengl;
using namespace audio;
using namespace audio::instrument;

namespace blockgame
{
	PlayScene::PlayScene() : Scene(), lastDrawTicks(0), lastBlockMove(0), level(1), lines(0), score(0), blocks(NULL), background(NULL), fallingBlock(NULL), nextBlock(NULL)
	{
		// Initializing random seed
		srand( time(NULL) );
		
		// Creating game song
		Sine instrument( Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
		Silence silence( Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
		Song * song = new Song( 120, Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	
		song->mixNote( instrument, Note::getFrequency( 'E', false, 5 ), Note::Noire );
		song->mixNote( instrument, Note::getFrequency( 'B', false, 4 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'C', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'D', false, 5 ), Note::Noire );
		song->mixNote( instrument, Note::getFrequency( 'C', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'B', false, 4 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'A', false, 4 ), Note::Noire );
		song->mixNote( instrument, Note::getFrequency( 'A', false, 4 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'C', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'E', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'E', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'D', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'C', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'B', false, 4 ), Note::Noire );
		song->mixNote( instrument, Note::getFrequency( 'B', false, 4 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'C', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'D', false, 5 ), Note::Noire );
		song->mixNote( instrument, Note::getFrequency( 'E', false, 5 ), Note::Noire );
		song->mixNote( instrument, Note::getFrequency( 'C', false, 5 ), Note::Noire );
		song->mixNote( instrument, Note::getFrequency( 'A', false, 4 ), Note::Noire );
		song->mixNote( instrument, Note::getFrequency( 'A', false, 4 ), Note::Blanche );
		
		song->mixNote( instrument, Note::getFrequency( 'D', false, 5 ), Note::Noire );
		song->mixNote( instrument, Note::getFrequency( 'D', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'F', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'A', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'A', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'G', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'F', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'E', false, 5 ), Note::Noire, true );
		song->mixNote( instrument, Note::getFrequency( 'C', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'E', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'E', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'D', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'C', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'B', false, 4 ), Note::Noire );
		song->mixNote( instrument, Note::getFrequency( 'B', false, 4 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'C', false, 5 ), Note::Croche );
		song->mixNote( instrument, Note::getFrequency( 'D', false, 5 ), Note::Noire );
		song->mixNote( instrument, Note::getFrequency( 'E', false, 5 ), Note::Noire );
		song->mixNote( instrument, Note::getFrequency( 'C', false, 5 ), Note::Noire );
		song->mixNote( instrument, Note::getFrequency( 'A', false, 4 ), Note::Noire );
		song->mixNote( instrument, Note::getFrequency( 'A', false, 4 ), Note::Blanche );
		
		Mixer::get()->add( "BlockgameSong", song );
		delete song;
		Mixer::get()->setRepeat( "BlockgameSong", true );
		
		// Creating grid
		this->blocks = new Grid( 10.0f, 20.0f );
		this->background = new Grid( 10.0f, 20.0f );
		
		// Generating pieces
		this->fallingBlock = Piece::generate();
		this->nextBlock = Piece::generate();
	
		// Fill background vector
		Point2D position( 0.0f, 0.0f );
		Color backgroundColor( 1.0f, 1.0f, 1.0f );
		backgroundColor.setAlpha( 0.2f );
	
		for( unsigned int y = 0 ; y < 20 ; y++ )
		{
			for( unsigned int x = 0 ; x < 10 ; x++ )
			{
				position.moveTo( x, y );
				this->background->insert( new Block( position, backgroundColor ) );
			}
		}
		
		this->camera.getEye().moveTo( 0.0f, 0.0f, 70.0f );
		this->camera.getCenter().moveTo( 0.0f, 0.0f, 0.0f );
	
		this->updateLabels();
		this->lastBlockMove = SDL_GetTicks();
		
		Mixer::get()->play( "BlockgameSong" );
	}
	
	PlayScene::~PlayScene()
	{
		// Stopping song
		Mixer::get()->stop( "BlockgameSong" );
		
		if( this->fallingBlock != NULL )
			delete this->fallingBlock;
		
		if( this->nextBlock != NULL )
			delete this->nextBlock;
			
		if( this->blocks != NULL )
			delete this->blocks;
			
		if( this->background != NULL )
			delete this->background;
	}
	
	void PlayScene::updateScore()
	{
		this->score += ( 21 + ( 3 * this->level ) - ( this->blocks->getHeight() - this->fallingBlock->getY() - 1 ) );
	}
	
	void PlayScene::updateLabels()
	{
		this->levelStr.str( "" );
		this->levelStr << this->level;
		this->linesStr.str( "" );
		this->linesStr << this->lines;
		this->scoreStr.str( "" );
		this->scoreStr << this->score;
	}
	
	void PlayScene::switchBlocks()
	{
		this->blocks->insert( this->fallingBlock );
		delete this->fallingBlock;
		this->fallingBlock = this->nextBlock;
		this->nextBlock = Piece::generate();
	
		this->lines += this->blocks->deleteFullLines();
		this->level = ( this->lines / 10 ) + 1;
		
		// Increase music pitch
		// NO SOUND PITCH MODIFICATION (audio bug)
		//Mixer::get()->setPitch( "BlockgameSong", 1.00 + (this->level - 1) * 0.1 );
		
		this->updateLabels();
		
		if( this->fallingBlock->isInCollision( this->blocks ) )
		{
			this->running = false;
			this->nextScene = new ScoreScene( this->score );
		}
	}
	
	void PlayScene::handleEvent( SDL_Event * event )
	{
		switch( event->type )
		{
			case SDL_QUIT:
			{
				this->running = false;
				break;
			}
			
			case SDL_KEYDOWN:
			{
	            if( event->key.keysym.sym == SDLK_ESCAPE )
					this->running = false;
					
				else if( event->key.keysym.sym == SDLK_LEFT )
				{
					// Move the falling block on the left
					if( this->fallingBlock != NULL )
					{
						this->fallingBlock->moveBy( -1.0f, 0.0f );
						
						if( this->fallingBlock->isInCollision( this->blocks ) )
							this->fallingBlock->moveBy( 1.0f, 0.0f );
						else
							this->fallingBlock->correctPosition( this->blocks->getWidth(), this->blocks->getHeight() );
					}
				}
				else if( event->key.keysym.sym == SDLK_RIGHT )
				{
					// Move the falling block on the right
					if( this->fallingBlock != NULL )
					{
						this->fallingBlock->moveBy( 1.0f, 0.0f );
						
						if( this->fallingBlock->isInCollision( this->blocks ) )
							this->fallingBlock->moveBy( -1.0f, 0.0f );
						else
							this->fallingBlock->correctPosition( this->blocks->getWidth(), this->blocks->getHeight() );
					}
				}
				else if( event->key.keysym.sym == SDLK_UP )
				{
					// Rotate the falling block
					this->fallingBlock->rotate();
					
					// Impossible rotation
					if( this->fallingBlock->isInCollision( this->blocks ) )
						this->fallingBlock->rotate( true );
					
					this->fallingBlock->correctPosition( this->blocks->getWidth(), this->blocks->getHeight() );
				}
				else if( event->key.keysym.sym == SDLK_DOWN )
				{
					// Drop the falling block
					this->updateScore();
					
					while( !this->fallingBlock->isAtGround() && !this->fallingBlock->isInCollision( this->blocks ) )
						this->fallingBlock->moveBy( 0.0f, -1.0f );
					
					this->fallingBlock->moveBy( 0.0f, 1.0f );
					this->switchBlocks();
				}

				break;
			}
		}
	}
	
	void PlayScene::live( unsigned int ticks )
	{
		if( this->fallingBlock != NULL )
		{
			if( ticks - this->lastBlockMove > static_cast<unsigned int>( 500.0f - ((static_cast<float>( this->level ) - 1.0f) * 50.0f)) )
			{
				// Free-fall action
				this->fallingBlock->moveBy( 0.0f, -1.0f );
				
				if( this->fallingBlock->isAtGround() || this->fallingBlock->isInCollision( this->blocks ) )
				{					
					this->fallingBlock->moveBy( 0.0f, 1.0f );
					this->updateScore();
					
					this->switchBlocks();
				}
				
				this->lastBlockMove = ticks;
			}
		}
	}
	
	void PlayScene::render( unsigned int ticks )
	{
		if( ticks - this->lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			
			float screenRatio = static_cast<float>( Screen::get()->getWidth() ) / static_cast<float>( Screen::get()->getHeight() );
			float screenSize = 35.0f;
			this->camera.setOrthogonal( -1.0f * screenSize * screenRatio, screenSize * screenRatio, -1.0f * screenSize, screenSize, 1.0f, 100.0f );
			
			glMatrixMode( GL_MODELVIEW );
			this->camera.look();
			
			glMultMatrixf( Matrix::translation( -28.0f, -30.0f, 0.0f ).get() );
			this->background->render();
			this->blocks->render();
			
			// Falling block
			if( this->fallingBlock != NULL )
				this->fallingBlock->render();
				
			// Next block
			glMultMatrixf( Matrix::translation( 35.0f, -42.0f, 0.0f ).get() );
			if( this->nextBlock != NULL )
				this->nextBlock->render();
			
			// User Interface
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 80 ), "Score", 1.0f );
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 112 ), this->scoreStr.str(), 1.0f );
			
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 160 ), "Lines", 1.0f );
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 192 ), this->linesStr.str(), 1.0f );
			
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 240 ), "Level", 1.0f );
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 272 ), this->levelStr.str(), 1.0f );
			
			Font::get("bitmap")->render( Point2D( Screen::get()->getWidth() - 250, Screen::get()->getHeight() - 380 ), "Next", 1.0f );
			
			Screen::get()->render();
			this->lastDrawTicks = ticks;
		}
	}
}

