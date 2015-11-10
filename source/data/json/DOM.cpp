#include <data/json/DOM.h>

#include <iostream>
#include <sstream>
using namespace std;

namespace data
{
	namespace json
	{
		DOM::DOM( Object * object )
		{
			#ifdef DEBUG0
			cout << "[Json][DOM] Initialized new DOM" << endl;
			#endif
			
			if( object != NULL )
			{
				vector<string> keys = object->keys();
				
				for( vector<string>::iterator it = keys.begin() ; it != keys.end() ; it++ )
					this->set( *it, (*object)[ *it ] );
			}
		}
		
		DOM::~DOM()
		{
			#ifdef DEBUG0
			cout << "[Json][DOM] Cleaning DOM Objects..." << endl;
			#endif
			
			for( map<string, Json *>::const_iterator it = this->variables.begin() ; it != this->variables.end() ; it++ )
				DOM::free( it->second );
			
			#ifdef DEBUG0
			cout << "[Json][DOM] Deleted." << endl;
			#endif
		}
		
		void DOM::free( Json * value )
		{
			if( value != NULL )
			{
				if( value->getType() == Json::Array )
				{
					json::Array * array = static_cast<json::Array *>( value );
					
					while( array->length() > 0 )
						DOM::free( array->pop() );
				}
				
				delete value;
			}
		}
		
		void DOM::set( const string& path, Json * value )
		{
			if( value->getDOM() == NULL )
			{
				value->setDOM( this );
			}
			else if( value->getDOM() != this )
			{
				value = value->clone();
				value->setDOM( this );
			}
			
			this->variables[ path ] = value;
		}
		
		void DOM::set( const string& path, const string& value )
		{
			this->variables[ path ] = json::String::create( value, this );
		}
		
		void DOM::set( const string& path, const char * value )
		{
			this->variables[ path ] = json::String::create( value, this );
		}
		
		void DOM::set( const string& path, int value )
		{
			this->variables[ path ] = json::Number::create( static_cast<double>( value ), this );
		}
		
		void DOM::set( const string& path, double value )
		{
			this->variables[ path ] = json::Number::create( value, this );
		}
		
		void DOM::set( const string& path, bool value )
		{
			this->variables[ path ] = json::Boolean::create( value, this );
		}

		Json * DOM::get( const string& path )
		{
			vector<string> names;
			unsigned int lastDot = 0;
			unsigned int dot = 0;
			
			do
			{
				dot = path.find( ".", lastDot );
				
				if( dot == string::npos )
					dot = path.length();
				
				names.push_back( path.substr( lastDot, dot - lastDot ) );
					
				lastDot = dot + 1;
			}
			while( dot < path.length() );
			
			Json * element = NULL;
			vector<string>::iterator name = names.begin();
			
			if( name != names.end() && name->length() > 0 )
			{
				if( this->variables.find( *name ) != this->variables.end() )
				{
					element = this->variables[ *name ];
					
					while( ++name != names.end() )
					{
						if( element->getType() == Json::Object )
						{
							Object * obj = static_cast<Object *>( element );
							
							if( obj->hasKey( *name ) )
							{
								element = (*obj)[ *name ];
							}
							else
							{
								element = NULL;
							
								ostringstream err;
								err << "Variable \"" << *(--name) << "\" has no \"";
								err << *(++name) << "\" key.";
								throw err.str();
							}
						}
						else
						{
							element = NULL;
							
							ostringstream err;
							err << "Variable \"" << *(--name) << "\" is not an object.";
							throw err.str();
						}
					}
				}
				else
				{
					ostringstream err;
					err << "Variable \"" << *name << "\" is not defined.";
					throw err.str();
				}
			}
			else
			{
				throw string( "An empty path was specified." );
			}
			
			return element;
		}
		
		string DOM::toString() const
		{
			ostringstream text;
			
			for( map<string, Json *>::const_iterator it = this->variables.begin() ; it != this->variables.end() ; it++ )
			{
				text << "----" << endl;
				text << "Variable:\t" << it->first << endl;
				text << "Type:\t\t";
				
				switch( it->second->getType() )
				{
					case Json::Object:
						text << "object";
						break;
						
					case Json::Array:
						text << "array";
						break;
						
					case Json::Number:
						text << "number";
						break;
						
					case Json::String:
						text << "string";
						break;
						
					case Json::Boolean:
						text << "boolean";
						break;
						
					case Json::Null:
						text << "null";
						break;
						
					default:
						text << "unknown";
						break;
				}
				
				text << endl;
				text << "Value:\t\t" << it->second->toString() << endl;
			}
			
			return text.str();
		}
	}
}

