#ifndef __OPENGL_TEXTURED_RECTANGLE_H
#define __OPENGL_TEXTURED_RECTANGLE_H	1

#include <opengl/Rectangle.h>
#include <opengl/Tile.h>

#include <opengl/Program.h>
#include <opengl/ArrayBufferObject.h>
#include <opengl/ElementArrayBufferObject.h>

#include <vector>
using namespace std;

namespace opengl
{
	class TexturedRectangle : public Rectangle
	{
		protected:
			static unsigned int instances;
			static Program * program;
			static ArrayBufferObject * vertices;
			static ArrayBufferObject * textureCoordinates;
			static ElementArrayBufferObject * indices;
			
			Tile * tile;
			
			void initializeRendering();
		
		public:
			TexturedRectangle();
			TexturedRectangle( const TexturedRectangle& rectangle );
			TexturedRectangle( const Rectangle& rectangle, const string& textureResource );
			TexturedRectangle( unsigned width, unsigned height );
			TexturedRectangle( unsigned width, unsigned height, const string& textureResource );
			virtual ~TexturedRectangle();
			
			Tile * getTile();
			
			void prepareRendering( vector<Point3D>& vertices, vector<Point2D>& textureCoordinates, vector<unsigned short int>& indices ) const;
			static void render( vector<Point3D>& vertices, vector<Point2D>& textureCoordinates, vector<unsigned short int>& indices, Texture2D * texture, unsigned int textureUnit = 0 );
	};
}

#endif

