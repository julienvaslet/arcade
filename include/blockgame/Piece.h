#ifndef __BLOCKGAME_PIECE_H
#define __BLOCKGAME_PIECE_H	1

#include <blockgame/Blocks.h>

using namespace opengl;
using namespace std;

namespace blockgame
{
	class Piece : public Blocks
	{
		protected:
			
		public:
			Piece();
			~Piece();
			
			void moveTo( float x, float y );
			void moveBy( float dx, float dy );
			void correctPosition( float width, float height );
			void rotate( bool clockwise = false );
			
			float getX() const;
			float getY() const;
			
			bool isAtGround();
			
			static Piece * generate();
	};
}

#endif
