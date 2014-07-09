#ifndef __OPENGL_BUFFEROBJECT_H
#define __OPENGL_BUFFEROBJECT_H	1

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>

using namespace std;

namespace opengl
{
	class BufferObject
	{
		public:
			enum Frequency
			{
				Stream = 0,
				Static = 3,
				Dynamic = 6
			};
			
			enum Access
			{
				Draw = 0,
				Read = 1,
				Copy = 2
			};
		
		protected:
			GLuint buffer;
			
		public:
			BufferObject();
			virtual ~BufferObject();
			
			GLuint getBuffer();
			static GLenum getUsage( Frequency f, Access a );
	};
}

#endif
