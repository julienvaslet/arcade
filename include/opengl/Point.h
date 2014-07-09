#ifndef __OPENGL_POINT_H
#define __OPENGL_POINT_H	1

using namespace std;

namespace opengl
{
	class Point
	{	
		protected:
			float x;
			float y;
			float z;
			
		public:
			Point( float x = 0.0f, float y = 0.0f, float z = 0.0f );
			Point( const Point& point );
			virtual ~Point();
			
			void setX( float x );
			void setY( float y );
			void setZ( float z );
			
			void moveTo( float x, float y, float z );
			void moveBy( float dx, float dy, float dz );
			
			float getX() const;
			float getY() const;
			float getZ() const;
	};
}

#endif
