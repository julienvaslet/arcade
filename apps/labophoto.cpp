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
		camera = GphotoCamera::getCamera( cameras.begin()->first );
	}
	else
		Logger::get() << "No camera detected." << Logger::endl;
		
	if( camera != NULL )
	{
		camera->capture( "capture.jpg" );
	}
	
	GphotoCamera::destroyContext();
	Logger::destroy();
	
	return 0;
}

