#ifndef __OPENGL_TEXTURE2D_H
#define __OPENGL_TEXTURE2D_H	1

using namespace std;

#include <opengl/Texture.h>
#include <vector>

namespace opengl
{
	class Texture2D : public Texture
	{	
		protected:
			
		public:
			Texture2D();
			virtual ~Texture2D();
			
			virtual void bind( unsigned int unit = 0 ) const;
			virtual void setData( unsigned char * data, unsigned int width, unsigned int height, GLenum type = GL_RGB );
			virtual void setWrapMode( GLenum mode );
			virtual void setFiltering( GLenum minification, GLenum magnification );
	};
}

#endif
