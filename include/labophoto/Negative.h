#ifndef __LABOPHOTO_NEGATIVE_H
#define __LABOPHOTO_NEGATIVE_H	1

#include <opengl/TexturedRectangle.h>

using namespace opengl;

namespace labophoto
{
	class Negative : public TexturedRectangle
	{
		protected:
			static unsigned int instances;
			static Program * program;
			static ArrayBufferObject * vertices;
			static ArrayBufferObject * textureCoordinates;
			static ElementArrayBufferObject * indices;
			
			bool invertColors;
			
			void initializeRendering();
		
		public:
			Negative();
			virtual ~Negative();
			
			static void render( vector<Point3D>& vertices, vector<Point2D>& textureCoordinates, vector<unsigned short int>& indices, Texture2D * texture, unsigned int textureUnit = 0 );
	};
}

#endif
