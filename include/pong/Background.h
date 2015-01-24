#ifndef __PONG_BACKGROUND_H
#define __PONG_BACKGROUND_H	1

#include <opengl/ColoredRectangle.h>
#include <vector>

using namespace opengl;
using namespace std;

namespace pong
{
	class Background
	{
		protected:
			vector<ColoredRectangle *> walls;
		
		public:
			Background();
			~Background();
			
			void prepareRendering( vector<Point3D>& vertices, vector<Color>& colors, vector<unsigned short int>& indices );
	};
}

#endif

