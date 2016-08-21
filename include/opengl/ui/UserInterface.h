#ifndef __OPENGL_UI_USERINTERFACE_H
#define __OPENGL_UI_USERINTERFACE_H	1

#define OPENGL_UI_DEFAULT_FONT			"bitmap"

namespace opengl
{
	namespace ui
	{
		class UserInterface;
	}
}

#include <opengl/ui/Element.h>
#include <opengl/Font.h>
#include <opengl/Matrix.h>

#include <SDL2/SDL.h>

#include <map>
using namespace std;

namespace opengl
{
	namespace ui
	{
		class UserInterface
		{
			protected:
				map<string, Element *> elements;
				set<string> mouseoverElements;
				set<string> hiddenElements;
				string fontName;
				float fontSize;
				
			public:
				UserInterface();
				virtual ~UserInterface();
				
				void setFont( const string& font, float fontSize = 0.0f );
				Font * getFont();
				float getFontSize();
				
				void addElement( Element * element, bool hidden = false );
				//void showElement( const string& name );
				//void hideElement( const string& name );
				Element * getElement( const string& name );
				
				virtual void render( unsigned int ticks );
				bool dispatchEvent( const SDL_Event * event );
		};
	}
}

#endif

