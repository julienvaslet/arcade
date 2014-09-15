#ifndef __TOOLS_LOGGER_LOGGER_H
#define __TOOLS_LOGGER_LOGGER_H	1

#include <map>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

namespace tools
{
	namespace logger
	{
		class Logger;
		typedef Logger& (*LoggerFunction)( Logger& );
		
		class Logger
		{
			protected:
				static Logger * defaultLogger;
				static map<string,Logger *> instances;
		
				Logger( const string& name, bool defaultLogger = false );
				virtual ~Logger();
			
			public:
				static Logger& get();
				static Logger& get( const string& name );
				static void destroy();
				
				static Logger& endl( Logger& logger );
			
				virtual Logger& operator<<( const string& value ) = 0;
				
				template<class T> Logger& operator<<( T& value )
				{
					stringstream ss;
					ss << value;
					*this << ss.str();
					return *this;
				}
				
				Logger& operator<<( LoggerFunction func );
		};
	}
}

#endif

