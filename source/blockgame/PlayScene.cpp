#include <blockgame/PlayScene.h>
#include <blockgame/ScoreScene.h>
#include <blockgame/PlaySceneEventHandler.h>

#include <opengl/OpenGL.h>
#include <opengl/Font.h>
#include <opengl/Screen.h>
#include <opengl/Point2D.h>
#include <opengl/Color.h>

#include <controller/Controller.h>

#include <audio/Mixer.h>
#include <audio/Song.h>
#include <audio/instrument/Sine.h>
#include <audio/instrument/Silence.h>

#include <cstdlib>
#include <ctime>

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

using namespace opengl;
using namespace controller;
using namespace audio;
using namespace audio::instrument;

namespace blockgame
{
	PlayScene::PlayScene() : Scene(), lastDrawTicks(0), lastBlockMove(0), lastTickEvent(0), level(1), lines(0), score(0), blocks(NULL), background(NULL), fallingBlock(NULL), nextBlock(NULL)
	{
		Player * player = Player::get( "Player" );
		
		if( player != NULL )
			player->setEventHandler( new PlaySceneEventHandler( this ) );
			
		else
		{
			#ifdef DEBUG1
			Logger::get() << "[PlayScene] No player found. Exiting." << Logger::endl;
			#endif
			
			this->running = false;
		}
		
		// Initializing random seed
		srand( time(NULL) );
		
		// Creating game song
		/*Sine instrument( Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
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
		Mixer::get()->setRepeat( "BlockgameSong", true );*/
		
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
		
		//Mixer::get()->play( "BlockgameSong" );
	}
	
	PlayScene::~PlayScene()
	{
		// Stopping song
		//Mixer::get()->stop( "BlockgameSong" );
		
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
			
			#ifndef __PI__
			case SDL_KEYDOWN:
			{
	            if( event->key.keysym.sym == SDLK_ESCAPE )
					this->running = false;
				
				break;
			}
			#endif
			
			case SDL_JOYAXISMOTION:
			case SDL_JOYBUTTONUP:
			case SDL_JOYBUTTONDOWN:
			{
				Controller::handleEvent( event );
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
		
		if( ticks - this->lastTickEvent > 100 )
		{
			Controller::tickEvent( ticks );
			this->lastTickEvent = ticks;
		}
	}
	
	void PlayScene::render( unsigned int ticks )
	{
		if( ticks - this->lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			
			float screenRatio = static_cast<float>( Screen::get()->getWidth() ) / static_cast<float>( Screen::get()->getHeight() );
			float screenSize = 35.0f;
			
			Matrix projection = Matrix::ortho( -1.0f * screenSize * screenRatio, screenSize * screenRatio, -1.0f * screenSize, screenSize, 1.0f, 100.0f );
			Matrix modelview = Matrix::lookAt( camera.getEye().getX(), camera.getEye().getY(), camera.getEye().getZ(), camera.getCenter().getX(), camera.getCenter().getY(), camera.getCenter().getZ(), camera.getUp().getX(), camera.getUp().getY(), camera.getUp().getZ() );
			Matrix translation = Matrix::translation( -28.0f, -30.0f, 0.0f );
			modelview.multiply( translation );

			this->background->render( projection, modelview );
			this->blocks->render( projection, modelview );
			
			// Falling block
			if( this->fallingBlock != NULL )
				this->fallingBlock->render( projection, modelview );
				
			// Next block
			Matrix translation2 = Matrix::translation( 35.0f, -42.0f, 0.0f );
			modelview.multiply( translation2 );
			
			if( this->nextBlock != NULL )
				this->nextBlock->render( projection, modelview );
			
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
	
	void PlayScene::moveLeft()
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
	
	void PlayScene::moveRight()
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
	
	void PlayScene::moveDown()
	{
		// Drop the falling block
		this->updateScore();
		
		while( !this->fallingBlock->isAtGround() && !this->fallingBlock->isInCollision( this->blocks ) )
			this->fallingBlock->moveBy( 0.0f, -1.0f );
		
		this->fallingBlock->moveBy( 0.0f, 1.0f );
		this->switchBlocks();
	}
	
	void PlayScene::rotate( bool clockwise )
	{
		// Rotate the falling block
		this->fallingBlock->rotate( clockwise );
		
		// Impossible rotation
		if( this->fallingBlock->isInCollision( this->blocks ) )
			this->fallingBlock->rotate( !clockwise );
		
		this->fallingBlock->correctPosition( this->blocks->getWidth(), this->blocks->getHeight() );
	}
}

