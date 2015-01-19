#ifndef __PONG_BACKGROUND_H
#define __PONG_BACKGROUND_H	1

#include <opengl/ColoredRectangle.h>

using namespace opengl;

namespace pong
{
	class Background
	{
		protected:
			vector<ColoredRectangle *> walls;
		
		public:
			Background();
			~Background();
			
			void render();
	};
}

#endif

