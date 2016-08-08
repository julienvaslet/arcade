#include <opengl/ui/Element.h>

#include <vector>
using namespace std;

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace opengl
{
	namespace ui
	{
		map<unsigned int, Element *> Element::elements;
		
		Element::Element() : id(0)
		{
			if( Element::elements.empty() )
				this->id = 1;
			else
			{
				map<unsigned int, Element *>::reverse_iterator it = elements.rbegin();
				
				if( it->first < OPENGL_UI_MAX_ELEMENTS )
					this->id = it->first + 1;
					
				else
				{
					#ifdef DEBUG0
					Logger::get() << "[Element] /!\\ Max element id reached. Element ids should be regenerated." << Logger::endl;
					#endif
					
					unsigned int lastId = 0;
					
					for( map<unsigned int, Element *>::iterator it = elements.begin() ; it != elements.end() ; it++ )
					{
						if( it->first > lastId + 1 )
						{
							this->id = lastId + 1;
							break;
						}
						else
							lastId = it->first;
					}
				}
			}
			
			if( this->id > 0 )
			{
				Element::elements[this->id] = this;
				
				#ifdef DEBUG0
				Logger::get() << "[Element#" << this->id << "] Created." << Logger::endl;
				#endif
			}
			#ifdef DEBUG0
			else
			{
				#ifdef DEBUG0
				Logger::get() << "[Element#???] Element has been created but not referenced. Max simultaneous elements ("" OPENGL_UI_MAX_ELEMENTS "") has been reached." << Logger::endl;
				#endif
			}
			#endif
		}
		
		Element::~Element()
		{
			map<unsigned int, Element*>::iterator it = Element::elements.find( this->id );
			
			// Dereference elements from global list
			if( it != Element::elements.end() )
				Element::elements.erase( it );
			
			#ifdef DEBUG0
			Logger::get() << "[Element#" << this->id << "] Deleted." << Logger::endl;
			#endif
		}
				
		Element * Element::get( unsigned int id )
		{
			Element * element = NULL;
			
			map<unsigned int, Element*>::iterator it = Element::elements.find( id );
			
			if( it != Element::elements.end() )
				element = it->second;
			
			return element;
		}

		void Element::regenerateIds()
		{
			#ifdef DEBUG0
			Logger::get() << "[Element] /!\\ Regenerating ids." << Logger::endl;
			#endif

			vector<Element *> elements;
			unsigned int id = 1;
			
			for( map<unsigned int, Element *>::iterator it = Element::elements.begin() ; it != Element::elements.end() ; it++ )
				elements.push_back( it->second );
				
			Element::elements.clear();
			
			for( vector<Element *>::iterator it = elements.begin() ; it != elements.end() ; it++ )
			{
				#ifdef DEBUG0
				Logger::get() << "[Element#" << (*it)->id << "] New id is " << id << "." << Logger::endl;
				#endif
				
				(*it)->id = id;
				Element::elements[id++] = *it;
			}
		}
	}
}
