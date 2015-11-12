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
	
	string sScript = "var a = 1;\nb = 23;\nvar c = a + b;";
	
	Script * script = new Script( sScript );
	script->run();
	
	delete script;
	
	Logger::destroy();
	
	return 0;
}

