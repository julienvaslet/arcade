#include <blockgame/PlaySceneEventHandler.h>

namespace blockgame
{
	PlaySceneEventHandler::PlaySceneEventHandler( PlayScene * scene ) : scene(scene)
	{
	}
	
	PlaySceneEventHandler::~PlaySceneEventHandler()
	{
	}
	
	void PlaySceneEventHandler::handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp )
	{
		if( this->scene != NULL )
		{
			switch( button )
			{
				default:
					break;
					
				case Mapping::NoButton:
				{
					// Tick event
					break;
				}
				
				case Mapping::HorizontalAxis:
				{
					if( value >= Mapping::Pushed )
						this->scene->moveRight();
						
					else if( value <= Mapping::ReversePushed )
						this->scene->moveLeft();
					
					break;
				}
				
				case Mapping::VerticalAxis:
				{
					if( value >= Mapping::Pushed )
						this->scene->moveDown();
						
					break;
				}
					
				case Mapping::NorthButton:
				case Mapping::EastButton:
				case Mapping::SouthButton:
				case Mapping::WestButton:
				{
					if( value == Mapping::Released )
						this->scene->rotate();
						
					break;
				}
			}
		}
	}
}

