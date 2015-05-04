#ifndef __SNAKE_SNAKE_H
#define __SNAKE_SNAKE_H	1

#include <snake/Constants.h>
#include <snake/Map.h>

#include <opengl/ColoredRectangle.h>
#include <opengl/Point2D.h>
#include <opengl/Point3D.h>
#include <opengl/Color.h>
#include <opengl/Vector.h>
#include <vector>

using namespace opengl;
using namespace std;

namespace snake
{
	class Snake
	{
		protected:
			Point2D origin;
			Vector direction;
			bool directionHasChanged;
			Color color;
			vector<ColoredRectangle *> rectangles;
			
			void keepTheTailInArea();
		
		public:
			Snake();
			virtual ~Snake();
			
			void moveTo( int x, int y );
			void move( unsigned int times = 1 );
			void setDirection( int dx, int dy );
			
			void grow( unsigned int times = 1 );
			bool isBitingItsTail() const;
			bool isInCollision( const ColoredRectangle& rectangle ) const;
			bool isInCollision( const Map& map ) const;
			
			void prepareRendering( vector<Point3D>& vertices, vector<Color>& colors, vector<unsigned short int>& indices ) const;
	};
}

#endif

