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
		
		vector<string> Parser::readSymbols( const string& content, const string& separators, const string& spaces )
		{
			vector<string> parsedSymbols;
	
			// Parsing symbols
			unsigned int lastIndex = 0;
			bool parsingSpaces = false;
	
			for( unsigned int index = 0 ; index < content.length() ; index++ )
			{
				// Count lines
				if( content[index] == '\n' )
					this->lines.push_back( parsedSymbols.size() );
			
				if( separators.find( content[index] ) != string::npos || spaces.find( content[index] ) != string::npos )
				{
					if( index > lastIndex )
					{
						parsingSpaces = false;
						parsedSymbols.push_back( content.substr( lastIndex, index - lastIndex ) );
					}
			
					if( spaces.find( content[index] ) == string::npos )
					{
						parsingSpaces = false;
						parsedSymbols.push_back( string(1, content[index]) );
					}
					else if( !parsingSpaces )
					{
						parsingSpaces = true;
						parsedSymbols.push_back( " " );
					}
			
					lastIndex = index + 1;
				}
				else if( index + 1 == content.length() )
				{
					parsingSpaces = false;
					parsedSymbols.push_back( content.substr( lastIndex, index - lastIndex + 1 ) );
				}
			}
			
			// Add the last line end position
			if( parsedSymbols.size() != this->lines.back() )
				this->lines.push_back( parsedSymbols.size() );
			
			return parsedSymbols;
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

