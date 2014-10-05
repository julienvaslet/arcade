#ifndef __OPENGL_TEXTURE_H
#define __OPENGL_TEXTURE_H	1

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>

using namespace std;

namespace opengl
{
	class Texture
	{	
		protected:
			unsigned int id;
			
		public:
			Texture();
			virtual ~Texture();
		
			unsigned int getId() const;
			virtual void bind( unsigned int unit ) = 0;
			virtual void setWrapMode( GLenum mode ) = 0;
			virtual void setFiltering( GLenum minification, GLenum magnification ) = 0;
	};
}

#endif
