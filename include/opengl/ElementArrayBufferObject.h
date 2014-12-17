#ifndef __OPENGL_ELEMENTARRAYBUFFEROBJECT_H
#define __OPENGL_ELEMENTARRAYBUFFEROBJECT_H	1

#ifdef __PI__
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

#include <vector>

#include <opengl/BufferObject.h>

using namespace std;

namespace opengl
{
	class ElementArrayBufferObject : public BufferObject
	{
		protected:
			unsigned int size;
		
			void bind();
			
		public:
			ElementArrayBufferObject();
			virtual ~ElementArrayBufferObject();
			
			void setData( const vector<unsigned int>& data, Frequency frequency = Static, Access access = Draw );

			void draw( unsigned int count = 0 );
			void draw( OpenGL::DrawMode mode, unsigned int count = 0 );
	};
}

#endif
