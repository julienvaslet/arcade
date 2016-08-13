#ifndef __OPENGL_UI_LABEL_H
#define __OPENGL_UI_LABEL_H	1

#include <opengl/ui/Element.h>

#include <opengl/Point2D.h>
#include <opengl/Point3D.h>
#include <opengl/Color.h>

#include <vector>
using namespace std;

namespace opengl
{
	namespace ui
	{
		class Label : public Element
		{
			private:
				static vector<unsigned int> renderingIndices;
				static vector<Point3D> renderingPositions;
				static vector<Point2D> renderingTextureCoordinates;
				static vector<Color> renderingColors;
				
				// this boolean should be thinked again
				// how to also manage global renderFunctions cache?
				//static bool useRenderingCache;
				
			protected:
				string value;
				Color color;
			
			public:
				Label( const string& name, const string& value );
				virtual ~Label();
				
				virtual void prepareRendering( unsigned int ticks );
				static void render( unsigned int ticks );
		};
	}
}

#endif

