#ifndef __BLOCKGAME_PIECE_H
#define __BLOCKGAME_PIECE_H	1

#include <blockgame/Block.h>

#include <opengl/OpenGL.h>
#include <opengl/Color.h>
#include <opengl/ArrayBufferObject.h>
#include <opengl/ElementArrayBufferObject.h>

#include <vector>

using namespace opengl;
using namespace std;

namespace blockgame
{
	class Piece
	{
		protected:
			ArrayBufferObject * vertices;
			ArrayBufferObject * textureCoordinates;
			ArrayBufferObject * colors;
			ElementArrayBufferObject * indices;
		
			vector<Block *> blocks;
			
		public:
			Piece( unsigned int reserveBlocks = 4 );
			~Piece();
			
			void moveTo( float x, float y );
			void moveBy( float dx, float dy );
			void correctPosition( float width, float height );
			void rotate();
			
			float getX() const;
			float getY() const;
			
			void insert( Block * block );
			void insert( Piece * piece );
			void render();
			
			bool isAtGround();
			bool isInCollision( Piece * piece );
			
			static Piece * generate();
	};
}

#endif
