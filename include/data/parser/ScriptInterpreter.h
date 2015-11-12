#ifndef __DATA__PARSER__SCRIPT_INTERPRETER_H
#define __DATA__PARSER__SCRIPT_INTERPRETER_H	1

#include <data/parser/Parser.h>

namespace data
{
	namespace parser
	{
		class ScriptInterpreter : public Parser
		{
			protected:
				static unsigned int noNameScriptIncrement;
				
				string scriptName;
				// Script * (should be linked to a script)
			
				const string& getScriptName();
			
				bool assignation();
				bool name();
				
				bool value();
				bool number();
				
				bool operation();
				bool sum();
				bool substract();
				bool divide();
				bool multiply();
	
			public:
				ScriptInterpreter( const string& content, const string& name = "" );
				~ScriptInterpreter();
		
				bool run( bool noop = false );
		};
	}
}

#endif
