#include <blockgame/IntroScene.h>
#include <blockgame/PlayScene.h>

#include <opengl/OpenGL.h>
#include <opengl/Font.h>
#include <opengl/Screen.h>
#include <opengl/Point2D.h>
#include <opengl/Color.h>

using namespace opengl;

namespace blockgame
{
	IntroScene::IntroScene() : Scene(), lastDrawTicks(0), lastAlphaChangeTicks(0), increaseAlpha(false), textColor(1.0f, 1.0f, 1.0f)
	{
		this->textColor.setAlpha( 1.0f );
	}
	
	IntroScene::~IntroScene()
	{
	}
	
	void IntroScene::handleEvent( SDL_Event * event )
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
					
				else if( event->key.keysym.sym != SDLK_LEFT && event->key.keysym.sym != SDLK_RIGHT && event->key.keysym.sym != SDLK_UP && event->key.keysym.sym != SDLK_DOWN )
				{
					this->running = false;
					this->nextScene = new PlayScene();
				}

				break;
			}
		}
	}
	
	void IntroScene::live( unsigned int ticks )
	{
		if( ticks - this->lastAlphaChangeTicks > 30 )
		{
			float alpha = this->textColor.getAlpha();
			
			if( increaseAlpha )
			{
				alpha += 0.1f;
				
				if( alpha > 1.0f )
				{
					alpha = 1.0f;
					increaseAlpha = false;
				}
			}
			else
			{
				alpha -= 0.1f;
				
				if( alpha < 0.0f )
				{
					alpha = 0.0f;
					increaseAlpha = true;
				}
			}
			
			this->textColor.setAlpha( alpha );
			this->lastAlphaChangeTicks = ticks;
		}
	}
	
	void IntroScene::render( unsigned int ticks )
	{
		if( ticks - this->lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			
			string title = "Block Game !";
			vector<Color> titleColors;
			titleColors.push_back( Color( "ff0000" ) ); // B
			titleColors.push_back( Color( "00ff00" ) ); // l
			titleColors.push_back( Color( "0000ff" ) ); // o
			titleColors.push_back( Color( "ff00ff" ) ); // c
			titleColors.push_back( Color( "00ffff" ) ); // k
			titleColors.push_back( Color( "000000" ) ); // _
			titleColors.push_back( Color( "ffffff" ) ); // G
			titleColors.push_back( Color( "ff0000" ) ); // a
			titleColors.push_back( Color( "00ff00" ) ); // m
			titleColors.push_back( Color( "0000ff" ) ); // e
			titleColors.push_back( Color( "000000" ) ); // _
			titleColors.push_back( Color( "ff00ff" ) ); // !
			
			Point2D origin;
			Font::get("bitmap")->renderSize( origin, title, 2.0f );
			origin.moveTo( (Screen::get()->getWidth() - origin.getX()) / 2.0f, Screen::get()->getHeight() - 150 );
			
			for( unsigned int i = 0 ; i < title.length() ; i++ )
			{
				Font::get("bitmap")->render( origin, title.substr( i, 1 ), titleColors[i], 2.0f );
				origin.moveBy( Font::get("bitmap")->renderWidth( title.substr( i, 1 ), 2.0f ), 0.0f );
			}	
			
			unsigned int width = Font::get("bitmap")->renderWidth( "Press any button to start", 1.0f );
			Font::get("bitmap")->render( Point2D( (Screen::get()->getWidth() - width) / 2.0f, 200 ), "Press any button to start", this->textColor, 1.0f );
			
			Screen::get()->render();
			
			this->lastDrawTicks = ticks;
		}
	}
}

