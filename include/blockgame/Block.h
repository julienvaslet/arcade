#ifndef __BLOCKGAME_BLOCK_H
#define __BLOCKGAME_BLOCK_H	1

#include <opengl/OpenGL.h>
#include <opengl/Color.h>
#include <opengl/Program.h>
#include <opengl/Texture2D.h>
#include <opengl/ArrayBufferObject.h>
#include <opengl/ElementArrayBufferObject.h>

using namespace opengl;

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
		
			Color color;
			
		public:
			Block( const Color& color );
			~Block();
			
			void prepareRendering( Point2D& position, vector<Point3D>& vPoints, vector<Point2D>& vTexCoords, vector<Color>& vColors, vector<unsigned int>& vIndices ) const;
			static void renderBlocks( vector<Point3D>& vPoints, vector<Point2D>& vTexCoords, vector<Color>& vColors, vector<unsigned int>& vIndices );
	};
}

#endif
