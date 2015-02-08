#ifndef __BLOCKGAME_BLOCKS_H
#define __BLOCKGAME_BLOCKS_H	1

#include <blockgame/Block.h>

#include <opengl/OpenGL.h>
#include <opengl/Color.h>
#include <opengl/ArrayBufferObject.h>
#include <opengl/ElementArrayBufferObject.h>
#include <opengl/Matrix.h>

#include <vector>

using namespace opengl;
using namespace std;

namespace blockgame
{
	class Blocks
	{
		protected:
			vector<Block *> blocks;
			Point3D origin;
			
		public:
			Blocks( unsigned int reserveBlocks = 4 );
			~Blocks();
			
			Point3D& getOrigin();
			
			void prepareRendering( vector<Point3D>& vPoints, vector<Point2D>& vTexCoords, vector<Color>& vColors, vector<unsigned short int>& vIndices );
			
			void insert( Block * block );
			void insert( Blocks * blocks );
			bool isInCollision( Blocks * blocks );
	};
}

#endif
