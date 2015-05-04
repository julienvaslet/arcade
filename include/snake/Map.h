#ifndef __SNAKE_MAP_H
#define __SNAKE_MAP_H	1

#include <opengl/Point3D.h>
#include <opengl/Color.h>
#include <opengl/ColoredRectangle.h>
#include <vector>
#include <string>

using namespace opengl;
using namespace std;

namespace snake
{
	class Map
	{
		protected:
			string title;
			Point2D snakeOrigin;
			vector<ColoredRectangle *> blocks;
		
		public:
			Map( const string& title );
			~Map();
			
			const vector<ColoredRectangle *>& getMap() const;
			
			bool isInCollision( const ColoredRectangle& rectangle ) const;
			
			const string& getTitle() const;
			const Point2D& getSnakeOrigin() const;
			void prepareRendering( vector<Point3D>& vertices, vector<Color>& colors, vector<unsigned short int>& indices ) const;
			
			static vector<string> getAvailableMaps();
	};
}

#endif

