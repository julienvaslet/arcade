#ifndef __OPENGL_CAMERA_H
#define __OPENGL_CAMERA_H	1

#include <opengl/Point.h>
#include <opengl/Vector.h>

using namespace std;

namespace opengl
{
	class Camera
	{	
		protected:
			Point eye;
			Point center;
			Vector up;
			
		public:
			Camera();
			virtual ~Camera();
			
			Point& getEye();
			Point& getCenter();
			Vector& getUp();
			
			void look() const;
			
			void setOrthogonal( float left, float right, float bottom, float top, float near, float far );
			void setPerspective( float fovy, float aspect, float near, float far );
			void setFrustum( float left, float right, float bottom, float top, float near, float far );
	};
}

#endif
