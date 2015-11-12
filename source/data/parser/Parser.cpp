#include <data/parser/Parser.h>
#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

namespace data
{
	namespace parser
	{
		Parser::Parser()
		{
		}

		Parser::~Parser()
		{
		}
		
		void Parser::readSymbols( const string& content, const string& separators, const string& spaces )
		{	
			// Parsing symbols
			unsigned int lastIndex = 0;
			bool parsingSpaces = false;
	
			for( unsigned int index = 0 ; index < content.length() ; index++ )
			{
				// Count lines
				if( content[index] == '\n' )
					this->lines.push_back( this->symbols.size() );
			
				if( separators.find( content[index] ) != string::npos || spaces.find( content[index] ) != string::npos )
				{
					if( index > lastIndex )
					{
						parsingSpaces = false;
						this->symbols.push_back( content.substr( lastIndex, index - lastIndex ) );
					}
			
					if( spaces.find( content[index] ) == string::npos )
					{
						parsingSpaces = false;
						this->symbols.push_back( string(1, content[index]) );
					}
					else if( !parsingSpaces )
					{
						parsingSpaces = true;
						this->symbols.push_back( " " );
					}
			
					lastIndex = index + 1;
				}
				else if( index + 1 == content.length() )
				{
					parsingSpaces = false;
					this->symbols.push_back( content.substr( lastIndex, index - lastIndex + 1 ) );
				}
			}
			
			// Add the last line end position
			if( this->symbols.size() != this->lines.back() )
				this->lines.push_back( this->symbols.size() );
		}
		
		void Parser::packSymbols( const vector<string>& combinations )
		{
			vector<string> packedSymbols;
			unsigned int currentLine = 0;
			unsigned int currentDeletedSymbols = 0;
			
			for( vector<string>::iterator it = this->symbols.begin() ; it != this->symbols.end() ; it++ )
			{
				bool symbolAppended = false;
				
				// Only lonely symbols are processed. (faster)
				if( (*it).length() == 1 )
				{
					for( vector<string>::const_iterator combination = combinations.begin() ; combination != combinations.end() ; combination++ )
					{
						unsigned int i = 0;
						bool combinationFound = true;
					
						for( i = 0 ; i < (*combination).length() ; i++, it++ )
						{
							if( it == this->symbols.end() || (*it).length() != 1 || (*combination).at(i) != (*it).at(0) )
							{
								combinationFound = false;
								break;
							}
						}
					
						if( combinationFound )
						{
							packedSymbols.push_back( *combination );
							currentDeletedSymbols += (*combination).length() - 1;
						}
						else
							it -= i;
					}
				}
				
				if( !symbolAppended )
				{
					packedSymbols.push_back( *it );
					
					if( it - this->symbols.begin() >= this->lines[currentLine] )
					{
						this->lines[currentLine++] -= currentDeletedSymbols;
						currentDeletedSymbols = 0;
					}
				}
			}
			
			this->symbols.swap( packedSymbols );
		}

		bool Parser::eop()
		{
			return this->pointer == this->symbols.end();
		}
		
		void Parser::initPointer()
		{
			this->pointer = this->symbols.begin();
		}
		
		void Parser::pushPointer()
		{
			this->pointers.push( this->pointer );
		}
		
		void Parser::popPointer( bool setActive )
		{
			if( ! this->pointers.empty() )
			{
				if( setActive )
					this->pointer = this->pointers.top();
				
				this->pointers.pop();
			}
		}

		int Parser::eatSpaces()
		{
			int spaces = 0;
	
			while( !this->eop() && this->findSymbol( " " ) )
			{
				spaces++;
				this->nextSymbol();
			}
	
			return spaces;
		}

		void Parser::nextSymbol( int count )
		{
			this->pointer += count;
		}

		void Parser::prevSymbol( int count )
		{
			this->pointer -= count;
		}

		bool Parser::findSymbol( const string& symbol )
		{
			return this->pointer != this->symbols.end() && *(this->pointer) == symbol;
		}

		string Parser::read()
		{
			return *(this->pointer);
		}
		
		unsigned int Parser::getLinesCount()
		{
			return this->lines.size();
		}
		
		unsigned int Parser::getCurrentLine()
		{
			unsigned int currentLine = 1;
			unsigned int currentSymbol = this->pointer - this->symbols.begin();
			
			for( vector<unsigned int>::iterator it = this->lines.begin() ; it != this->lines.end() ; it++, currentLine++ )
			{
				if( currentSymbol <= *it )
					break;
			}
			
			return currentLine;
		}
		
		string Parser::getCurrentLineText()
		{
			string line;
			stringstream ss;
			
			unsigned int currentLine = this->getCurrentLine();
			unsigned int firstSymbol = 0;
			unsigned int lastSymbol = this->lines[currentLine - 1];
			
			if( currentLine > 1 )
				firstSymbol = this->lines[currentLine - 2];
				
			for( unsigned int i = firstSymbol ; i <= lastSymbol ; i++ )
			{
				// Ignoring leading spaces
				if( this->symbols[i] == " " && ss.rdbuf()->in_avail() == 0 )
					continue;
					
				ss << this->symbols[i];
			}
			
			return ss.str();
		}
	}
}

