#ifndef __OPENGL_SHADER_H
#define __OPENGL_SHADER_H	1

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>

using namespace std;

namespace opengl
{
	class Shader
	{
		protected:
			
		public:
			Shader();
			virtual ~Shader();
	};
}

#endif

