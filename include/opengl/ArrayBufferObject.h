#ifndef __OPENGL_ARRAYBUFFEROBJECT_H
#define __OPENGL_ARRAYBUFFEROBJECT_H	1

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>
#include <vector>

#include <opengl/BufferObject.h>
#include <opengl/Point.h>
#include <opengl/Color.h>

using namespace std;

namespace opengl
{
	class ArrayBufferObject : public BufferObject
	{
		protected:
			
			void bind();
			
		public:
			ArrayBufferObject();
			virtual ~ArrayBufferObject();
			
			void setData( const vector<float>& data, Frequency frequency = Static, Access access = Draw );
			void setData( const vector<Point>& data, Frequency frequency = Static, Access access = Draw );
			void setData( const vector<Color>& data, Frequency frequency = Static, Access access = Draw );
			
			void bindColorPointer( unsigned int stride = 0 );
			void bindVertexPointer( unsigned int stride = 0 );
	};
}

#endif
