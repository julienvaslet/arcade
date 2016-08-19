#ifndef __OPENGL_UI_ELEMENT_H
#define __OPENGL_UI_ELEMENT_H	1

#include <opengl/Rectangle.h>

namespace opengl
{
	namespace ui
	{
		class Element;
	}
}

#include <opengl/ui/UserInterface.h>

#include <string>
#include <map>
#include <set>
using namespace std;

namespace opengl
{
	namespace ui
	{
		class Element;
		typedef bool (*Event)( Element * );
		typedef void (*ElementRenderFunction)( unsigned int );
		
		class Element
		{
			protected:
				static set<ElementRenderFunction> renderFunctions;
				
				UserInterface * ui;
				string name;
				
				Rectangle rectangle;
				bool disabledState;
				map<string, vector<Event> > events;
			
			public:
				Element( const string& name );
				virtual ~Element();
				
				const string& getName() const;
				void setUserInterface( UserInterface * ui );
				Rectangle& getRectangle();
				
				void addEventHandler( const string& event, Event callback );
				void removeEventHandler( const string& event );
				void trigger( const string& event );
				bool isDisabled() const;
				void setDisabledState( bool state );
				
				virtual void prepareRendering( unsigned int ticks ) = 0;
				static void render( unsigned int ticks );
		};
	}
}

#endif

