#include <tools/logger/Stdout.h>
#include <data/script/Script.h>
#include <string>

using namespace tools::logger;
using namespace data::script;
using namespace std;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	Script * script = Script::load( "data/scripts/variables.js" );
	
	if( script != NULL )
	{
		script->run();
		Logger::get() << "DOM status:" << Logger::endl << script->getDOM()->toString() << Logger::endl;
		
		delete script;
	}
	
	Logger::destroy();
	
	return 0;
}

