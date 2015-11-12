#ifndef __DATA__SCRIPT__SCRIPT_H
#define __DATA__SCRIPT__SCRIPT_H	1

#include <data/parser/ScriptInterpreter.h>
#include <data/json/DOM.h>
#include <string>

using namespace std;
using namespace data::json;
using namespace data::parser;

namespace data
{
	namespace script
	{
		class Script
		{
			protected:
				DOM * dom;
				ScriptInterpreter * interpreter;
				
			public:
				Script( const string& code );
				virtual ~Script();
				
				bool run();
				bool check();
		};
	}
}

#endif

