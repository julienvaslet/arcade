#include <tools/logger/Stdout.h>
#include <tools/camera/GphotoCamera.h>

using namespace tools::logger;
using namespace tools::camera;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	GphotoCamera * camera = NULL;
	GphotoCamera::initializeContext();
	
	map<string,string> cameras;
	GphotoCamera::list( cameras );
	
	if( cameras.size() > 0 )
	{
		Logger::get() << "------------" << Logger::endl;
		Logger::get() << "Camera list:" << Logger::endl;
		
		for( map<string,string>::iterator it = cameras.begin() ; it != cameras.end() ; it++ )
			Logger::get() << it->first << ": " << it->second << Logger::endl;
		
		Logger::get() << "------------" << Logger::endl;
		
		camera = GphotoCamera::getCamera( cameras.begin()->first );
	}
	else
		Logger::get() << "No camera detected." << Logger::endl;
		
	if( camera != NULL )
	{
		vector<string> settings;
		
		camera->listSettings( settings );
		
		Logger::get() << "Settings list:" << Logger::endl;
		
		vector<string> settingValues;
		
		for( vector<string>::iterator it = settings.begin() ; it != settings.end() ; it++ )
		{
			string currentValue = camera->getSetting( *it );
			
			if( currentValue.length() > 0 )
			{
				Logger::get() << *it << ": " << Logger::endl;
				
				settingValues.clear();
				camera->getSettingValues( *it, settingValues );
				
				if( settingValues.size() > 0 )
				{
					for( vector<string>::iterator it2 = settingValues.begin() ; it2 != settingValues.end() ; it2++ )
						Logger::get() << "  " << ( *it2 == currentValue ? "+" : "-" ) << " " << *it2 << Logger::endl;
				}
				else
				{
					Logger::get() << "    " << currentValue << Logger::endl;
				}
			}
		}
		
		camera->preview( "preview.jpg" );
		camera->capture( "capture.jpg" );
	}
	
	GphotoCamera::destroyContext();
	Logger::destroy();
	
	return 0;
}

