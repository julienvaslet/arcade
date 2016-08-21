#include <opengl/ui/Element.h>
#include <opengl/ui/UserInterface.h>

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
		set<ElementRenderFunction> Element::renderFunctions;
		
		Element::Element( const string& name ) : ui(NULL), name(name), disabledState(false)
		{
		}
		
		Element::~Element()
		{
		}
		
		const string& Element::getName() const
		{
			return this->name;
		}
		
		bool Element::isDisabled() const
		{
			return this->disabledState;
		}
		
		void Element::setDisabledState( bool state )
		{
			this->disabledState = state;
		}
		
		void Element::setUserInterface( UserInterface * ui )
		{
			this->ui = ui;
		}
		
		Rectangle& Element::getRectangle()
		{
			return this->rectangle;
		}
		
		void Element::render( unsigned int ticks )
		{
			for( set<ElementRenderFunction>::iterator it = Element::renderFunctions.begin() ; it != Element::renderFunctions.end() ; it++ )
				(*it)( ticks );
				
			Element::renderFunctions.clear();
		}
		
		void Element::addEventHandler( const string& event, Event callback )
		{
			map<string, vector<Event> >::iterator it = this->events.find( event );
		
			if( it == this->events.end() )
			{
				vector<Event> evts;
				evts.push_back( callback );
				this->events[event] = evts;
			}
			else
				this->events[event].push_back( callback );
		}
		
		void Element::removeEventHandler( const string& event )
		{
			map<string, vector<Event> >::iterator it = this->events.find( event );
		
			if( it != this->events.end() )
				this->events.erase( it );
		}
		
		void Element::trigger( const string& event )
		{
			map<string, vector<Event> >::iterator it = this->events.find( event );
		
			if( it != this->events.end() )
			{
				#ifdef DEBUG0
				cout << "[Element#" << this->getName() << "] Triggered event \"" << event << "\"." << endl;
				#endif
			
				for( vector<Event>::reverse_iterator itEvent = it->second.rbegin() ; itEvent != it->second.rend() ; itEvent++ )
				{
					if( *itEvent != NULL )
					{
						// This could trigger a bad behavior if events are badly setted.
						if( (*itEvent)( this ) == false )
						{
							#ifdef DEBUG0
							cout << "[Element#" << this->getName() << "] Event propagation stopped." << endl;
							#endif
						
							break;
						}
					}
					else
						break;
				}
			}
		}
	}
}