#include <pong/PlaySceneEventHandler.h>

namespace pong
{
	PlaySceneEventHandler::PlaySceneEventHandler( PlayScene * scene, unsigned int player ) : scene(scene), player(player)
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
					unsigned int stateTimestamp = controller->getStateTimestamp( Mapping::VerticalAxis );
					short int verticalAxis = controller->getState( Mapping::VerticalAxis );
					
					if( timestamp - stateTimestamp > 15 )
					{
						if( verticalAxis == Mapping::Pushed )
						{
							this->scene->moveDown( this->player );
						}
						else if( verticalAxis == Mapping::ReversePushed )
						{
							this->scene->moveUp( this->player );
						}
					}
					
					break;
				}
				
				case Mapping::VerticalAxis:
				{
					if( value == Mapping::Pushed )
					{
						this->scene->moveDown( this->player );
					}
					else if( value == Mapping::ReversePushed )
					{
						this->scene->moveUp( this->player );
					}
						
					break;
				}
				
				case Mapping::NorthButton:
				case Mapping::SouthButton:
				case Mapping::WestButton:
				case Mapping::EastButton:
				{
					if( value == Mapping::Released )
						this->scene->buttonAction( this->player );
						
					break;
				}
			}
		}
	}
}

