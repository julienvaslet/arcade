#include <tools/logger/Logger.h>

#ifdef DEBUG0
#include <iostream>
#endif

using namespace std;

namespace tools
{
	namespace logger
	{
		Logger * Logger::defaultLogger = NULL;
		map<string, Logger*> Logger::instances;
	
		Logger::Logger( const string& name, bool defaultLogger )
		{
			if( defaultLogger )
				Logger::defaultLogger = this;
				
			map<string, Logger*>::iterator it = Logger::instances.find( name );
			
			if( it != Logger::instances.end() )
			{
				delete it->second;
				Logger::instances[name] = this;
				
				#ifdef DEBUG0
				cout << "[Logger#" << this << "] Replaced Logger \"" << name << "\"." << std::endl;
				#endif
			}
			else
			{
				Logger::instances[name] = this;
				
				#ifdef DEBUG0
				cout << "[Logger#" << this << "] Created logger \"" << name << "\"." << std::endl;
				#endif
			}
			
			#ifdef DEBUG0
			if( defaultLogger )
				cout << "[Logger#" << this << "] Is now the default logger." << std::endl;
			#endif
		}
		
		Logger::~Logger()
		{
			#ifdef DEBUG0
			string name;
			#endif
			
			if( this == Logger::defaultLogger )
				Logger::defaultLogger = NULL;

			for( map<string, Logger*>::iterator it = Logger::instances.begin() ; it != Logger::instances.end() ; it++ )
			{
				if( it->second == this )
				{
					#ifdef DEBUG0
					name = it->first;
					#endif
					
					Logger::instances.erase( it );
					
					#ifdef DEBUG0
					cout << "[Logger#" << this << "] Deleted logger \"" << name << "\"." << std::endl;
					#endif
					break;
				}
			}
		}
		
		Logger& Logger::get()
		{
			if( Logger::defaultLogger == NULL )
				throw "No default logger has been declared.";
				
			return *(Logger::defaultLogger);
		}
		
		Logger& Logger::get( const string& name )
		{
			map<string, Logger*>::iterator it = Logger::instances.find( name );
			
			if( it == Logger::instances.end() )
			{
				#ifdef DEBUG0
				cout << "[Logger] Logger not found, redirecting to default logger." << std::endl;
				#endif
				
				return Logger::get();
			}
			else
				return *(it->second);
		}
		
		void Logger::destroy()
		{
			for( map<string, Logger*>::iterator it = Logger::instances.begin() ; it != Logger::instances.end() ; it++ )
				delete it->second;

			Logger::instances.clear();
		}
		
		Logger& Logger::operator<<( LoggerFunction func )
		{
			return func(*this);
		}
		
		Logger& Logger::endl( Logger& logger )
		{
			logger << "\n";
			// flush?
			return logger;
		}
	}
}

