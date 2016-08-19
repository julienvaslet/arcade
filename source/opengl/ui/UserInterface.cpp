#include <opengl/ui/UserInterface.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace opengl
{
	namespace ui
	{
		UserInterface::UserInterface() : fontName(OPENGL_UI_DEFAULT_FONT), fontSize(0.0f)
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
		
		void UserInterface::setFont( const string& font, float fontSize )
		{
			this->fontName = font;
			this->fontSize = fontSize;
			
			#ifdef DEBUG0
			Logger::get() << "[UserInterface] Interface font is \"" << this->fontName << "\" (size: " << this->fontSize << ")" << Logger::endl;
			
			if( this->getFont() == NULL )
				Logger::get() << "[UserInterface] /!\\ Font \"" << this->fontName << "\" seems to not be loaded." << Logger::endl;
			#endif
		}
		
		float UserInterface::getFontSize()
		{
			return this->fontSize;
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
		
		Element * UserInterface::getElement( const string& name )
		{
			Element * element = NULL;
			
			map<string, Element *>::iterator it = this->elements.find( element->getName() );
			
			if( it != this->elements.end() )
				element = it->second;
				
			return element;
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

