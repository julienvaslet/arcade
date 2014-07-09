#ifndef __OPENGL_VECTOR_H
#define __OPENGL_VECTOR_H	1

using namespace std;

namespace opengl
{
	class Vector
	{	
		protected:
			float x;
			float y;
			float z;
			
		public:
			Vector();
			Vector( const Vector& vector );
			Vector( float x, float y, float z );
			~Vector();
		
			float getX();
			float getY();
			float getZ();
		
			void show() const;
		
			float norm() const;
			Vector& normalize();
		
			Vector& operator*=( float n );
			Vector& operator*=( const Vector& vector );
			Vector operator*( const Vector& vector );
			Vector& operator/=( float n );
			Vector operator/( float n );
	};
}

#endif
