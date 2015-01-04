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
			
		public:
			Blocks( unsigned int reserveBlocks = 4 );
			~Blocks();
			
			void render( Matrix& projection, Matrix& modelview );
			
			void insert( Block * block );
			void insert( Blocks * blocks );
			bool isInCollision( Blocks * blocks );
	};
}

#endif
