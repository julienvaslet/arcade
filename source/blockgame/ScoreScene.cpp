#include <blockgame/ScoreScene.h>
#include <blockgame/IntroScene.h>

#include <opengl/OpenGL.h>
#include <opengl/Font.h>
#include <opengl/Screen.h>
#include <opengl/Point2D.h>
#include <opengl/Color.h>

#include <sstream>

using namespace opengl;

namespace blockgame
{
	ScoreScene::ScoreScene( unsigned int score ) : Scene(), lastDrawTicks(0), lastAlphaChangeTicks(0), increaseAlpha(false), textColor(1.0f, 1.0f, 1.0f)
	{
		this->textColor.setAlpha( 1.0f );
		stringstream ss;
		ss << "Your score is : " << score;
		this->score = ss.str();
	}
	
	ScoreScene::~ScoreScene()
	{
	}
	
	void ScoreScene::handleEvent( SDL_Event * event )
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
					this->nextScene = new IntroScene();
				}

				break;
			}
		}
	}
	
	void ScoreScene::live( unsigned int ticks )
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
	
	void ScoreScene::render( unsigned int ticks )
	{
		if( ticks - this->lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			
			unsigned int width = Font::get("bitmap")->renderWidth( this->score, 1.5f );
			Font::get("bitmap")->render( Point2D( (Screen::get()->getWidth() - width) / 2.0f, Screen::get()->getHeight() - 150 ), this->score, 1.5f );
			
			width = Font::get("bitmap")->renderWidth( "Press any button to quit", 1.0f );
			Font::get("bitmap")->render( Point2D( (Screen::get()->getWidth() - width) / 2.0f, 200 ), "Press any button to quit", this->textColor, 1.0f );
			
			Screen::get()->render();
			
			this->lastDrawTicks = ticks;
		}
	}
}

