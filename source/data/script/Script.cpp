#include <data/script/Script.h>

namespace data
{
	namespace script
	{
		Script::Script( const string& code ) : dom(NULL), interpreter(NULL)
		{
			this->dom = new DOM( NULL );
			this->interpreter = new ScriptInterpreter( code );
		}
		
		Script::~Script()
		{
			if( this->interpreter != NULL )
				delete this->interpreter;
				
			if( this->dom != NULL )
				delete this->dom;
		}
	
		bool Script::run()
		{
			return this->interpreter->run();
		}
		
		bool Script::check()
		{
			return this->interpreter->run( true );
		}
	}
}

