#ifndef __OPENGL_RECTANGLE_H
#define __OPENGL_RECTANGLE_H	1

#include <opengl/Point2D.h>
#include <opengl/Point3D.h>

#include <vector>
using namespace std;

namespace opengl
{
	class Rectangle
	{
		protected:
			Point3D origin;
			Point2D anchor;
			unsigned int width;
			unsigned int height;
		
		public:
			Rectangle();
			Rectangle( const Rectangle& rectangle );
			Rectangle( unsigned width, unsigned height );
			virtual ~Rectangle();
			
			Point3D& getOrigin();
			Point2D& getAnchor();
			unsigned int getWidth();
			unsigned int getHeight();
			void setWidth( unsigned int width );
			void setHeight( unsigned int height );
			void resize( unsigned int width, unsigned int height );
			void resizeBy( int deltaWidth, unsigned int deltaHeight );
			bool isInCollision( Rectangle& rectangle );
			
			void prepareRendering( vector<Point3D>& vertices, vector<unsigned short int>& indices, bool clockwise = true ) const;
	};
}

#endif

