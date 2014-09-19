#ifndef __OPENGL_SHADER_H
#define __OPENGL_SHADER_H	1

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>

#include <string>

using namespace std;

namespace opengl
{
	class Shader
	{
		public:
			enum Type
			{
				Vertex = GL_VERTEX_SHADER,
				Fragment = GL_FRAGMENT_SHADER
			};
		
		protected:
			GLuint id;
			string getShaderTypeString() const;
			
		public:
			Shader( Shader::Type type );
			virtual ~Shader();
			
			bool getCompileStatus() const;
			bool getDeleteStatus() const;
			virtual Shader::Type getShaderType() const;
			int getInfoLogLength() const;
			int getShaderSourceLength() const;
			string getInfoLog() const;
			
			unsigned int getId() const;
			bool load( const string& filename );
			bool compile( const string& content );
	};
}

#endif

