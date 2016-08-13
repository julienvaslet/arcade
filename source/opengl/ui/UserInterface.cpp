#include <opengl/ui/UserInterface.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace opengl
{
	namespace ui
	{
		UserInterface::UserInterface() : fontName(OPENGL_UI_DEFAULT_FONT)
		{
			#ifdef DEBUG0
			Logger::get() << "[UserInterface] Created." << Logger::endl;
			#endif
		}
		
		UserInterface::~UserInterface()
		{
			for( map<string, Element *>::iterator it = this->elements.begin() ; it != this->elements.end() ; it++ )
				delete it->second;
			
			this->elements.clear();
			
			#ifdef DEBUG0
			Logger::get() << "[UserInterface] Deleted." << Logger::endl;
			#endif
		}
		
		void UserInterface::setFont( const string& font )
		{
			this->fontName = font;
		}
		
		Font * UserInterface::getFont()
		{
			return Font::get( this->fontName );
		}

		void UserInterface::addElement( Element * element, bool hidden )
		{
			map<string, Element *>::iterator it = this->elements.find( element->getName() );
			
			if( it != this->elements.end() )
			{
				#ifdef DEBUG0
				Logger::get() << "[UserInterface] /!\\ Element#" << element->getName() << " will be replaced by a new one." << Logger::endl;
				#endif
				
				delete it->second;
			}
			
			this->elements[element->getName()] = element;
			element->setUserInterface( this );
			
			#ifdef DEBUG0
			Logger::get() << "[UserInterface] Element#" << element->getName() << " has been added." << Logger::endl;
			#endif
		}
		
		void UserInterface::render( unsigned int ticks )
		{
			for( map<string, Element *>::iterator it = this->elements.begin() ; it != this->elements.end() ; it++ )
				it->second->prepareRendering( ticks );
				
			Element::render( ticks );
			
			this->getFont()->render();
		}
	}
}

