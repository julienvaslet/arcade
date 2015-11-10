#ifndef __DATA_JSON_DOM
#define __DATA_JSON_DOM	1

#include <data/json/Json.h>
#include <string>
#include <vector>
#include <map>
using namespace std;

namespace data
{
	namespace json
	{
		class Object;
	
		class DOM
		{
			protected:
				map<string, Json *> variables;
				
				static void free( Json * value = NULL );
			
			public:
				DOM( Object * object = NULL );
				~DOM();
				
				//bool exists( const string& path ) const;
				
				void set( const string& path, Json * value );
				void set( const string& path, const string& value );
				void set( const string& path, const char * value );
				void set( const string& path, int value );
				void set( const string& path, double value );
				void set( const string& path, bool value );
				
				Json * get( const string& path );
				
				string toString() const;
		};
	}
}

#endif

