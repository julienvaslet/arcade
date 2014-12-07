#include <data/parser/IniParser.h>
#include <algorithm>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace data
{
	namespace parser
	{
		IniParser::IniParser( const string& content )
		{
			string charSymbols = ";=[]\n";
			vector<string> parsedSymbols = this->readSymbols( content, charSymbols.c_str(), " \t\r" );
			
			#ifdef DEBUG0
			Logger::get() << "[IniParser] Loaded (" << parsedSymbols.size() << " symbols)." << Logger::endl;
			#endif
		}
		
		IniParser::~IniParser()
		{
		}

		ini::Configuration * IniParser::parse()
		{
			this->initPointer();
			this->configuration = new ini::Configuration();
			this->currentSection = NULL;

			while( !this->eop() )
			{
				if( this->comment() ) {}
				else if( this->section() ) {}
				else if( this->assignation() ) {}
				else
				{
					#ifdef DEBUG0
					Logger::get() << "[IniParser] Warning: ignoring symbol \"" << this->read() << "\"." << Logger::endl;
					#endif
					
					this->nextSymbol();
				}
			}

			return this->configuration;
		}
		
		bool IniParser::section()
		{
			bool bReturn = false;
			this->pushPointer();
			
			this->eatSpaces();
	
			if( this->findSymbol( "<!--" ) )
			{
				this->nextSymbol();
				string content;

				while( !this->findSymbol( "-->" ) )
				{
					content += this->read();
					this->nextSymbol();
				}
		
				this->nextSymbol();
				//this->currentNode->append( new node::Node( node::Node::Comment, content ) );
				bReturn = true;
			}
			
			this->popPointer( !bReturn );
	
			return bReturn;
		}
		
		bool IniParser::comment()
		{
			bool bReturn = false;
			this->pushPointer();
			
			//...
			
			this->popPointer( !bReturn );
	
			return bReturn;
		}
		
		bool IniParser::assignation()
		{
			bool bReturn = false;
			this->pushPointer();
			
			//...
			
			this->popPointer( !bReturn );
	
			return bReturn;
		}
		
		bool IniParser::key()
		{
			bool bReturn = false;
			this->pushPointer();
			
			//...
			
			this->popPointer( !bReturn );
	
			return bReturn;
		}
		
		bool IniParser::value()
		{
			bool bReturn = false;
			this->pushPointer();
			
			//...
			
			this->popPointer( !bReturn );
	
			return bReturn;
		}
	}
}

