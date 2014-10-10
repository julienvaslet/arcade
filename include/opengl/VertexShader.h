#ifndef __OPENGL_VERTEXSHADER_H
#define __OPENGL_VERTEXSHADER_H	1

#include <opengl/Shader.h>

#include <SDL2/SDL_opengl.h>

using namespace std;

namespace opengl
{
	class VertexShader : public Shader
	{
		protected:
			
		public:
			VertexShader();
			virtual ~VertexShader();
			
			virtual Shader::Type getShaderType() const;
	};
}

#endif

