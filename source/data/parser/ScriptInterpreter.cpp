#include <data/parser/ScriptInterpreter.h>
#include <algorithm>
#include <sstream>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

using namespace std;

namespace data
{
	namespace parser
	{
		unsigned int ScriptInterpreter::noNameScriptIncrement = 1;
		
		ScriptInterpreter::ScriptInterpreter( const string& content, const string& name ) : scriptName(name)
		{
			if( this->scriptName.length() == 0 )
			{
				stringstream ss;
				ss << "script#" << ScriptInterpreter::noNameScriptIncrement++;
				this->scriptName = ss.str();
			}
		
			string charSymbols = ";+-/*=(),.{}[]&|\"':\\/";
			vector<string> parsedSymbols = this->readSymbols( content, charSymbols.c_str(), " \t\r\n" );
			
			// repack functions todo (in order to keep good lines markers)
			// \" \' && ++ -- += -= *= /= == != <= >= || // /* */
			
			// Packing symbols
			for( vector<string>::iterator it = parsedSymbols.begin() ; it != parsedSymbols.end() ; it++ )
			{
				bool symbolAppended = false;
		
				// Symbols: \" & \'
				if( *it == "\\" )
				{
					if( *(++it) == "\"" )
					{
						this->symbols.push_back( "\\\"" );
						symbolAppended = true;
					}
					else if( *(++it) == "'" )
					{
						this->symbols.push_back( "\\'" );
						symbolAppended = true;
					}
					else
					 it -= 1;
				}
		
				if( !symbolAppended )
					this->symbols.push_back( *it );
			}
	
			#ifdef DEBUG0
			Logger::get() << "[Script][" << this->getScriptName() << "] Loaded (" << this->symbols.size() << " symbols, " << this->getLinesCount() << " lines)." << Logger::endl;
			#endif
		}
		
		ScriptInterpreter::~ScriptInterpreter()
		{
		}
		
		const string& ScriptInterpreter::getScriptName()
		{
			return this->scriptName;
		}

		bool ScriptInterpreter::run( bool noop )
		{
			this->initPointer();
			
			while( !this->eop() )
			{
				this->eatSpaces();
				
				if( this->assignation() ) {}
				else
				{
					Logger::get() << "[Script][" << this->getScriptName() << "][Error] Unknown symbol \"" << this->read() << "\" on line " << this->getCurrentLine() << ": " << this->getCurrentLineText() << Logger::endl;
					
					break;
				}
			}
			
			return this->eop();
		}
		
		/*
		 * Variables
		 */
		
		bool ScriptInterpreter::assignation()
		{
			bool bReturn = false;
			this->pushPointer();
			
			bool varTermUsed = false;
			
			// Optionnal "var" term
			if( this->findSymbol( "var" ) )
			{
				varTermUsed = true;
				this->nextSymbol();
			}
				
			if( this->name() )
			{
				if( this->findSymbol( "=" ) )
				{
					if( this->value() )
					{
						bReturn = true;
					}
				}
				else if( this->findSymbol( "*=" ) )
				{
				}
				else if( this->findSymbol( "+=" ) )
				{
				}
				else if( this->findSymbol( "-=" ) )
				{
				}
				else if( this->findSymbol( "/=" ) )
				{
				}
			}
			else if( varTermUsed )
			{
				Logger::get() << "[ScriptInterpreter] Incorrect variable name." << Logger::endl;
				
				// should exit the script execution
			}
			
			this->popPointer( !bReturn );
			return bReturn;
		}
		
		bool ScriptInterpreter::name()
		{
			bool bReturn = false;
			this->pushPointer();
			
			
			
			this->popPointer( !bReturn );
			return bReturn;
		}
		
		/*
		 * Values
		 */
		
		bool ScriptInterpreter::value()
		{
			bool bReturn = false;
			this->pushPointer();
			
			if( this->number() )
			{
				bReturn = true;
			}
			
			this->popPointer( !bReturn );
			return bReturn;
		}
		
		bool ScriptInterpreter::number()
		{
			bool bReturn = false;
			double value = 0.0;
			bool negative = false;
			
			this->pushPointer();
			this->eatSpaces();
			
			if( this->findSymbol( "-" ) )
			{
				negative = true;
				this->nextSymbol();
			}
				
			if( !this->eop() )
			{
				bool valid = true;
				string txt = this->read();
				double decimal = 1.0;
				
				for( string::reverse_iterator rit = txt.rbegin() ; rit != txt.rend() ; rit++ )
				{
					if( *rit >= '0' && *rit <= '9' )
					{
						value += (*rit - '0') * decimal;
					}
					else
					{
						valid = false;
						break;
					}
					
					decimal *= 10.0;
				}
				
				if( valid )
				{
					this->nextSymbol();
					
					if( this->findSymbol( "." ) )
					{
						this->nextSymbol();
						
						if( !this->eop() )
						{
							double floatPart = 0.0;
							valid = true;
							txt = this->read();
							decimal = 0.1;
							
							for( string::iterator it = txt.begin() ; it != txt.end() ; it++ )
							{
								if( *it >= '0' && *it <= '9' )
								{
									floatPart += (*it - '0') * decimal;
								}
								else
								{
									valid = false;
									break;
								}
				
								decimal *= 0.1;
							}
							
							if( valid )
							{
								this->nextSymbol();
								value += floatPart;
							}
							else
								this->prevSymbol();
						}
						else
						 this->prevSymbol();
					}
					
					if( negative )
						value *= -1.0;
					
					bReturn = true;
					Logger::get() << "value is : " << value << Logger::endl;
					//this->currentElement = json::Number::create( value );
				}
			}
			
			this->popPointer( !bReturn );
			
			return bReturn;
		}
		
		/*
		 * Operations
		 */
		 
		bool ScriptInterpreter::operation()
		{
			return false;
		}
		
		bool ScriptInterpreter::sum()
		{
			return false;
		}
		
		bool ScriptInterpreter::substract()
		{
			return false;
		}
		
		bool ScriptInterpreter::divide()
		{
			return false;
		}
		
		bool ScriptInterpreter::multiply()
		{
			return false;
		}
	}
}

