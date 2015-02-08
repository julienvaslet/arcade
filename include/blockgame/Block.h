#ifndef __BLOCKGAME_BLOCK_H
#define __BLOCKGAME_BLOCK_H	1

#include <opengl/OpenGL.h>
#include <opengl/Color.h>
#include <opengl/Matrix.h>
#include <opengl/Program.h>
#include <opengl/Texture2D.h>
#include <opengl/ArrayBufferObject.h>
#include <opengl/ElementArrayBufferObject.h>
#include <opengl/TexturedRectangle.h>

#include <game/Resource.h>

using namespace opengl;
using namespace game;

namespace blockgame
{
	class Block
	{
		protected:
			static unsigned int instances;
			static Program * program;
			static Texture2D * texture;
			static ArrayBufferObject * vertices;
			static ArrayBufferObject * textureCoordinates;
			static ArrayBufferObject * colors;
			static ElementArrayBufferObject * indices;
		
			Point2D position;
			TexturedRectangle * rectangle;
			Color color;
			
		public:
			Block( const Point2D& position, const Color& color );
			~Block();
			
			Point2D& getPosition();
			void prepareRendering( Point3D& origin, vector<Point3D>& vPoints, vector<Point2D>& vTexCoords, vector<Color>& vColors, vector<unsigned short int>& vIndices );
			
			static void render( vector<Point3D>& vPoints, vector<Point2D>& vTexCoords, vector<Color>& vColors, vector<unsigned short int>& vIndices );
	};
}

#endif
