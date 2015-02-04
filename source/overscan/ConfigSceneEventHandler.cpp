#include <overscan/ConfigSceneEventHandler.h>

namespace overscan
{
	ConfigSceneEventHandler::ConfigSceneEventHandler( ConfigScene * scene ) : scene(scene)
	{
	}
	
	ConfigSceneEventHandler::~ConfigSceneEventHandler()
	{
	}
	
	void ConfigSceneEventHandler::handleEvent( Controller * controller, Mapping::Button button, short int value, unsigned int timestamp )
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
				
				case Mapping::VerticalAxis:
				{
					if( value == Mapping::Pushed )
					{
						this->scene->moveDown();
					}
					else if( value == Mapping::ReversePushed )
					{
						this->scene->moveUp();
					}
						
					break;
				}
				
				case Mapping::HorizontalAxis:
				{
					if( value == Mapping::Pushed )
					{
						this->scene->moveRight();
					}
					else if( value == Mapping::ReversePushed )
					{
						this->scene->moveLeft();
					}
						
					break;
				}
				
				case Mapping::NorthButton:
				case Mapping::SouthButton:
				case Mapping::WestButton:
				case Mapping::EastButton:
				{
					if( value == Mapping::Released )
						this->scene->nextStep();
						
					break;
				}
			}
		}
	}
}

