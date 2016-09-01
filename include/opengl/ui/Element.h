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
#include <opengl/ui/event/Event.h>

#include <string>
#include <map>
#include <set>
using namespace std;

namespace opengl
{
	namespace ui
	{
		class Element;
		typedef bool (*EventHandler)( Element *, const event::Event * );
		typedef void (*ElementRenderFunction)( unsigned int );
		
		class Element
		{
			protected:
				static set<ElementRenderFunction> renderFunctions;
				
				UserInterface * ui;
				string name;
				
				Rectangle rectangle;
				bool disabledState;
				map<string, vector<EventHandler> > events;
			
			public:
				Element( const string& name );
				virtual ~Element();
				
				const string& getName() const;
				virtual void setUserInterface( UserInterface * ui );
				UserInterface * getUserInterface();
				Rectangle& getRectangle();
				
				void addEventHandler( const string& event, EventHandler callback );
				void removeEventHandler( const string& event );
				void trigger( const event::Event * event );
				void trigger( const string& action );
				
				bool isDisabled() const;
				void setDisabledState( bool state );
				
				virtual void prepareRendering( unsigned int ticks ) = 0;
				static void render( unsigned int ticks );
		};
	}
}

#endif

