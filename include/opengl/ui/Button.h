#ifndef __OPENGL_UI_BUTTON_H
#define __OPENGL_UI_BUTTON_H	1

#include <opengl/ui/Element.h>

#include <opengl/Program.h>
#include <opengl/ArrayBufferObject.h>
#include <opengl/ElementArrayBufferObject.h>

#include <opengl/Point2D.h>
#include <opengl/Point3D.h>
#include <opengl/Color.h>

#include <vector>
using namespace std;

namespace opengl
{
	namespace ui
	{
		class Button : public Element
		{
			private:
				static unsigned int instances;
				static Program * program;
				static ArrayBufferObject * vertices;
				static ArrayBufferObject * colors;
				static ElementArrayBufferObject * indices;
				
				static vector<unsigned short int> renderingIndices;
				static vector<Point3D> renderingVertices;
				static vector<Color> renderingColors;
				
				void initializeRendering();
				
			protected:
				string value;
				Color textColor;
				Color buttonColor;
				bool highlighted;
				bool pushed;
				
				void autoResize();
			
			public:
				Button( const string& name, const string& value );
				virtual ~Button();
				
				const string& getValue() const;
				void setValue( const string& value, bool resize = false );
				
				void setTextColor( const string& color );
				void setTextColor( const Color& color );
				const Color& getTextColor() const;
				
				void setButtonColor( const string& color );
				void setButtonColor( const Color& color );
				const Color& getButtonColor() const;
				
				virtual void prepareRendering( unsigned int ticks );
				static void render( unsigned int ticks );
			
				static bool eventMouseDown( Element * element );
				static bool eventMouseUp( Element * element );
				static bool eventMouseEnter( Element * element );
				static bool eventMouseLeave( Element * element );
		};
	}
}

#endif
