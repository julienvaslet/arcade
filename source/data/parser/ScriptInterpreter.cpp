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
		
		ScriptInterpreter::ScriptInterpreter( const string& content, Script * script, const string& name ) : scriptName(name), script(script), currentValue(NULL), noop(false)
		{
			if( this->scriptName.length() == 0 )
			{
				stringstream ss;
				ss << "script#" << ScriptInterpreter::noNameScriptIncrement++;
				this->scriptName = ss.str();
			}
		
			string charSymbols = ";+-/*=(),.{}[]&|\"':\\/";
			this->readSymbols( content, charSymbols.c_str(), " \t\r\n" );
			
			vector<string> combinations;
			combinations.push_back( "\\\"" );
			combinations.push_back( "\\'" );
			combinations.push_back( "&&" );
			combinations.push_back( "||" );
			combinations.push_back( "++" );
			combinations.push_back( "--" );
			combinations.push_back( "+=" );
			combinations.push_back( "-=" );
			combinations.push_back( "*=" );
			combinations.push_back( "/=" );
			combinations.push_back( "==" );
			combinations.push_back( "!=" );
			combinations.push_back( "<=" );
			combinations.push_back( ">=" );
			combinations.push_back( "//" );
			combinations.push_back( "/*" );
			combinations.push_back( "*/" );
			
			// This packing method should be tested.
			this->packSymbols( combinations );
	
			#ifdef DEBUG0
			Logger::get() << "[Script][" << this->getScriptName() << "] Loaded (" << this->symbols.size() << " symbols, " << this->getLinesCount() << " lines)." << Logger::endl;
			#endif
			
			this->reservedNames.push_back( "var" );
			this->reservedNames.push_back( "if" );
			this->reservedNames.push_back( "else" );
			this->reservedNames.push_back( "for" );
			this->reservedNames.push_back( "do" );
			this->reservedNames.push_back( "while" );
			this->reservedNames.push_back( "continue" );
			this->reservedNames.push_back( "break" );
			this->reservedNames.push_back( "typeof" );
			this->reservedNames.push_back( "true" );
			this->reservedNames.push_back( "false" );
			this->reservedNames.push_back( "null" );
			this->reservedNames.push_back( "return" );
			this->reservedNames.push_back( "function" );
			this->reservedNames.push_back( "switch" );
			this->reservedNames.push_back( "case" );
			this->reservedNames.push_back( "default" );
			this->reservedNames.push_back( "arguments" );
			this->reservedNames.push_back( "this" );
			this->reservedNames.push_back( "in" );
			this->reservedNames.push_back( "try" );
			this->reservedNames.push_back( "catch" );
			this->reservedNames.push_back( "finally" );
		}
		
		ScriptInterpreter::~ScriptInterpreter()
		{
			this->initStacks();
		}
		
		const string& ScriptInterpreter::getScriptName()
		{
			return this->scriptName;
		}
		
		void ScriptInterpreter::initStacks()
		{
			/*
			 * Values' stack
			 */
			 
			while( !this->stackedValues.empty() )
			{
				json::Json * value = this->stackedValues.top();
				
				if( value != this->currentValue && value != NULL && value->getDOM() == NULL )
					delete value;
				
				this->stackedValues.pop();
			}
			
			// Delete the variable if it is not handled by a DOM
			if( this->currentValue != NULL && this->currentValue->getDOM() == NULL )
				delete this->currentValue;
			
			this->currentValue = NULL;
			
			/*
			 * Names' stack
			 */
			 
			while( !this->stackedNames.empty() )
				this->stackedNames.pop();
				
			this->currentName = "";
		}
		
		void ScriptInterpreter::pushValue()
		{
			this->stackedValues.push( this->currentValue );
		}
		
		void ScriptInterpreter::popValue( bool setActive )
		{
			if( !this->stackedValues.empty() )
			{
				if( setActive )
				{
					// Delete the variable if it is not handled by a DOM
					if( this->currentValue != NULL && this->currentValue->getDOM() == NULL )
						delete this->currentValue;
					
					this->currentValue = this->stackedValues.top();
				}
		
				this->stackedValues.pop();
			}
			#ifdef DEBUG0
			else
				this->handleError( "Unable to unstack value (empty stack)" );
			#endif
		}
		
		void ScriptInterpreter::pushName()
		{
			this->stackedNames.push( this->currentName );
		}
		
		void ScriptInterpreter::popName( bool setActive )
		{
			if( !this->stackedNames.empty() )
			{
				if( setActive )
					this->currentName = this->stackedNames.top();
			
				this->stackedNames.pop();
			}
			#ifdef DEBUG0
			else
				this->handleError( "Unable to unstack name (empty stack)" );
			#endif
		}
		
		void ScriptInterpreter::handleError( const string& message, bool abort )
		{
			Logger::get() << "[Script][" << this->getScriptName() << "][Error] " << message << " on line " << this->getCurrentLine() << ": " << this->getCurrentLineText() << Logger::endl;
			
			if( abort )
			{
				this->pointer = this->symbols.end();
				
				// Cleaning pointers stack
				while( !this->pointers.empty() )
					this->pointers.pop();
			}
		}
		
		bool ScriptInterpreter::run( bool noop )
		{
			this->initPointer();
			this->initStacks();
			
			while( !this->eop() )
			{				
				if( this->assignation() ) {}
				else if( !this->eop() )
					this->handleError( "Unknown symbol", true );
					
				this->eatSpaces();
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
			
			this->eatSpaces();
			
			if( this->name() )
			{
				this->eatSpaces();
				
				if( this->findSymbol( "=" ) )
				{
					this->nextSymbol();
					
					if( this->expression() )
					{
						this->eatSpaces();
						
						if( this->findSymbol( ";" ) )
						{
							this->nextSymbol();
							
							//if( !this->noop )
								this->script->dom->set( this->currentName, this->currentValue );
							
							bReturn = true;
						}
						else
						{
							this->handleError( "Missing \";\" after variable assignation", true );
						}
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
				else if( varTermUsed && this->findSymbol( ";" ) )
				{
					this->nextSymbol();
					
					//if( !this->noop )
						this->script->dom->set( currentName, json::Null::create() );
					
					bReturn = true;
				}
			}
			else if( varTermUsed )
			{
				this->handleError( "Incorrect variable name", true );
			}
			
			this->popPointer( !bReturn );
			return bReturn;
		}
		
		bool ScriptInterpreter::name()
		{
			bool bReturn = false;
			bool isReservedName = false;
			
			this->pushPointer();
			this->eatSpaces();
			
			string name = this->read();
			
			// Check if the used name is not a reserved one
			for( vector<string>::const_iterator it = this->reservedNames.begin() ; it != this->reservedNames.end() ; it++ )
			{
				if( name == *it )
				{
					isReservedName = true;
					break;
				}
			}
			
			if( !isReservedName )
			{
				this->nextSymbol();
				this->currentName = name;
				
				bReturn = true;
			}
			
			this->popPointer( !bReturn );
			return bReturn;
		}
		
		bool ScriptInterpreter::expression()
		{
			bool bReturn = false;
			this->pushPointer();
			
			if( this->value() )
			//if( this->operation() || this->value() )
				bReturn = true;
			
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
			
			this->eatSpaces();
			
			if( this->null() || this->boolean() || this->text() || this->number() )
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

					this->currentValue = json::Number::create( value );
					
					bReturn = true;
				}
			}
			
			this->popPointer( !bReturn );
			
			return bReturn;
		}
		
		bool ScriptInterpreter::null()
		{
			bool bReturn = false;
			
			this->pushPointer();
			this->eatSpaces();
			
			if( this->findSymbol( "null" ) )
			{
				this->nextSymbol();
				this->currentValue = json::Null::create();
				
				bReturn = true;
			}
			
			this->popPointer( !bReturn );
			
			return bReturn;
		}
		
		bool ScriptInterpreter::boolean()
		{
			bool bReturn = false;
			
			this->pushPointer();
			this->eatSpaces();
			
			if( this->findSymbol( "true" ) )
			{
				this->nextSymbol();
				this->currentValue = json::Boolean::create( true );
				
				bReturn = true;
			}
			else if( this->findSymbol( "false" ) )
			{
				this->nextSymbol();
				this->currentValue = json::Boolean::create( false );
				
				bReturn = true;
			}
			
			this->popPointer( !bReturn );
			
			return bReturn;
		}
		
		bool ScriptInterpreter::text()
		{
			bool bReturn = false;
			
			this->pushPointer();
			this->eatSpaces();
			
			if( this->findSymbol( "\"" ) )
			{
				this->nextSymbol();
				
				stringstream ss;
				
				while( !this->eop() && !this->findSymbol( "\"" ) )
				{
					ss << this->read();
					this->nextSymbol();
				}
				
				if( this->findSymbol( "\"" ) )
				{
					this->nextSymbol();
					this->currentValue = json::String::create( ss.str() );
					
					bReturn = true;
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
			bool bReturn = false;
			this->pushPointer();
			
			/*if( this->sum() || this->substract() || this->divide() || this->multiply() )
			{
				bReturn = true;
			}*/
			
			this->popPointer( !bReturn );
			return bReturn;
		}
		
		bool ScriptInterpreter::sum()
		{
			bool bReturn = false;
			this->pushPointer();
			
			this->eatSpaces();
			
			if( this->value() )
			{
				this->eatSpaces();
				
				if( this->findSymbol( "+" ) )
				{
					this->nextSymbol();
					
					if( this->expression() )
					{
						bReturn = true;
					}
				}
			}
			
			this->popPointer( !bReturn );
			return bReturn;
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

