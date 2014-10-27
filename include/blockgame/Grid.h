#ifndef __BLOCKGAME_GRID_H
#define __BLOCKGAME_GRID_H	1

#include <blockgame/Blocks.h>

using namespace opengl;
using namespace std;

namespace blockgame
{
	class Grid : public Blocks
	{
		protected:
			float width;
			float height;
			
		public:
			Grid( float width, float height );
			~Grid();
			
			float getWidth();
			float getHeight();
			
			unsigned int deleteFullLines();
	};
}

#endif
