#ifndef __OPENGL_FRAGMENTSHADER_H
#define __OPENGL_FRAGMENTSHADER_H	1

#include <opengl/Shader.h>

#include <SDL2/SDL_opengl.h>

using namespace std;

namespace opengl
{
	class FragmentShader : public Shader
	{
		protected:
			
		public:
			FragmentShader();
			virtual ~FragmentShader();
			
			virtual Shader::Type getShaderType() const;
	};
}

#endif

