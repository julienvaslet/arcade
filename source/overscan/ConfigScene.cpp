#include <overscan/ConfigScene.h>
#include <overscan/ConfigSceneEventHandler.h>
#include <controller/Controller.h>
#include <opengl/Font.h>
#include <opengl/Screen.h>

#include <tools/pi/Overscan.h>
#include <sstream>

using namespace controller;
using namespace opengl;
using namespace tools::pi;

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace overscan
{
	ConfigScene::ConfigScene() : Scene(), lastDrawTicks(0), top(0), left(0), right(0), bottom(0), step(0)
	{
		Overscan::get( &(this->left), &(this->right), &(this->top), &(this->bottom) );
		
		Color white( "ffffff" );		
		ColoredRectangle * rectangle = NULL;

		// Top
		rectangle = new ColoredRectangle( Screen::get()->getWidth(), 10, white );
		rectangle->getOrigin().moveTo( 0.0f, 0.0f, 0.0f );
		this->rectangles.push_back( rectangle );
		
		// Left
		rectangle = new ColoredRectangle( 10, Screen::get()->getHeight(), white );
		rectangle->getOrigin().moveTo( 0.0f, 0.0f, 0.0f );
		this->rectangles.push_back( rectangle );
		
		// Right
		rectangle = new ColoredRectangle( Screen::get()->getWidth(), 10, white );
		rectangle->getOrigin().moveTo( 0.0f, Screen::get()->getHeight() - 10, 0.0f );
		this->rectangles.push_back( rectangle );
		
		// Bottom
		rectangle = new ColoredRectangle( 10, Screen::get()->getHeight(), white );
		rectangle->getOrigin().moveTo( Screen::get()->getWidth() - 10, 0.0f, 0.0f );
		this->rectangles.push_back( rectangle );
		
		this->updateStrings();
		this->initializeStep();
		
		// Initialize controllers event handler
		for( unsigned int i = 0 ; i < Player::getCount() ; i++ )
			Player::get( i )->setEventHandler( new ConfigSceneEventHandler( this ) );
	}
	
	ConfigScene::~ConfigScene()
	{
		for( vector<ColoredRectangle *>::iterator it = this->rectangles.begin() ; it != this->rectangles.end() ; it++ )
			delete *it;
			
		this->rectangles.clear();
	}
	
	void ConfigScene::updateStrings()
	{
		stringstream ss;
		
		ss.str( "" );
		ss << this->top;
		this->sTop = ss.str();
		
		ss.str( "" );
		ss << this->left;
		this->sLeft = ss.str();
		
		ss.str( "" );
		ss << this->right;
		this->sRight = ss.str();
		
		ss.str( "" );
		ss << this->bottom;
		this->sBottom = ss.str();
	}
	
	void ConfigScene::updateOverscan()
	{
		Overscan::set( &(this->left), &(this->right), &(this->top), &(this->bottom) );
		this->updateStrings();
	}
	
	void ConfigScene::initializeStep( int step )
	{
		// Top-left corner
		switch( step )
		{
			case 0:
			{
				this->instructions = "Make the top-left corner visible";
				
				
		
				// Left-top corner
				ColoredRectangle * rectangle = new ColoredRectangle( 10, 100 );
				rectangle->getOrigin().moveTo( 0.0f, 0.0f, 0.0f );
				rectangle->getColor().setColor( "ff0000" );
				this->rectangles.push_back( rectangle );
		
				rectangle = new ColoredRectangle( 100, 10 );
				rectangle->getOrigin().moveTo( 0.0f, 0.0f, 0.0f );
				rectangle->getColor().setColor( "ff0000" );
				this->rectangles.push_back( rectangle );
				
				this->step = 0;
				break;
			}
			
			case 1:
			{
				this->instructions = "Make the bottom-right corner visible";
		
				// Right-bottom corner
				ColoredRectangle * rectangle = new ColoredRectangle( 10, 100 );
				rectangle->getOrigin().moveTo( Screen::get()->getWidth() - 10, Screen::get()->getHeight() - 100, 0.0f );
				rectangle->getColor().setColor( "ff0000" );
				this->rectangles.push_back( rectangle );
		
				rectangle = new ColoredRectangle( 100, 10 );
				rectangle->getOrigin().moveTo( Screen::get()->getWidth() - 100, Screen::get()->getHeight() - 10, 0.0f );
				rectangle->getColor().setColor( "ff0000" );
				this->rectangles.push_back( rectangle );
				
				this->step = 1;
				break;
			}
			
			case 2:
			{
				this->running = false;
			}
		}
	}
	
	void ConfigScene::handleEvent( SDL_Event * event )
	{
		switch( event->type )
		{
			case SDL_QUIT:
			{
				this->running = false;
				break;
			}
		
			case SDL_JOYAXISMOTION:
			case SDL_JOYBUTTONUP:
			case SDL_JOYBUTTONDOWN:
			{
				Controller::handleEvent( event );
				break;
			}
		}
	}

	void ConfigScene::moveDown()
	{
		switch( this->step )
		{
			case 0:
			{
				this->top++;
				this->updateOverscan();
				break;
			}
			
			case 1:
			{
				this->bottom--;
				this->updateOverscan();
				break;
			}
		}
	}
	
	void ConfigScene::moveUp()
	{
		switch( this->step )
		{
			case 0:
			{
				this->top--;
				this->updateOverscan();
				break;
			}
			
			case 1:
			{
				this->bottom++;
				this->updateOverscan();
				break;
			}
		}
	}
	
	void ConfigScene::moveLeft()
	{
		switch( this->step )
		{
			case 0:
			{
				this->left--;
				this->updateOverscan();
				break;
			}
			
			case 1:
			{
				this->right++;
				this->updateOverscan();
				break;
			}
		}
	}
	
	void ConfigScene::moveRight()
	{
		switch( this->step )
		{
			case 0:
			{
				this->left++;
				this->updateOverscan();
				break;
			}
			
			case 1:
			{
				this->right--;
				this->updateOverscan();
				break;
			}
		}
	}
	
	void ConfigScene::nextStep()
	{
		this->step++;
		this->initializeStep( this->step );
	}
	
	void ConfigScene::previousStep()
	{
		if( this->step > 0 )
			this->step--;
			
		this->initializeStep( this->step );
	}

	void ConfigScene::live( unsigned int ticks )
	{
	}
	
	void ConfigScene::render( unsigned int ticks )
	{
		Point2D textSize;
		vector<Point3D> vertices;
		vector<Color> colors;
		vector<unsigned int short> indices;
		
		if( ticks - this->lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			
			for( vector<ColoredRectangle *>::iterator it = this->rectangles.begin() ; it != this->rectangles.end() ; it++ )
				(*it)->prepareRendering( vertices, colors, indices );
				
			ColoredRectangle::render( vertices, colors, indices );
			
			// Top
			textSize.moveTo( 0, 0 );
			Font::get("bitmap")->getTextSize( textSize, this->sTop );
			Font::get("bitmap")->write( Point2D( (Screen::get()->getWidth() - textSize.getX()) / 2.0f, 20 ), this->sTop );
			
			// Left
			textSize.moveTo( 0, 0 );
			Font::get("bitmap")->getTextSize( textSize, this->sLeft );
			Font::get("bitmap")->write( Point2D( 20, (Screen::get()->getHeight() - textSize.getY()) / 2.0f ), this->sLeft );
			
			// Right
			textSize.moveTo( 0, 0 );
			Font::get("bitmap")->getTextSize( textSize, this->sRight );
			Font::get("bitmap")->write( Point2D( Screen::get()->getWidth() - textSize.getX() - 20, (Screen::get()->getHeight() - textSize.getY()) / 2.0f ), this->sRight );
			
			// Bottom
			textSize.moveTo( 0, 0 );
			Font::get("bitmap")->getTextSize( textSize, this->sBottom );
			Font::get("bitmap")->write( Point2D( (Screen::get()->getWidth() - textSize.getX()) / 2.0f, Screen::get()->getHeight() - textSize.getY() - 20 ), this->sBottom );
			
			// Instructions
			textSize.moveTo( 0, 0 );
			Font::get("bitmap")->getTextSize( textSize, this->instructions );
			Font::get("bitmap")->write( Point2D( (Screen::get()->getWidth() - textSize.getX()) / 2.0f, (Screen::get()->getHeight() - textSize.getY()) / 2.0f ), this->instructions );
			
			Font::get("bitmap")->render();
			
			Screen::get()->render();
			
			this->lastDrawTicks = ticks;
		}
	}
}

