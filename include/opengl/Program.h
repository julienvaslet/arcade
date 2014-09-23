#ifndef __OPENGL_PROGRAM_H
#define __OPENGL_PROGRAM_H	1

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>

#include <opengl/VertexShader.h>
#include <opengl/FragmentShader.h>

#include <string>
#include <map>

using namespace std;

namespace opengl
{
	class Program
	{
		protected:
			GLuint id;
			map<Shader::Type, GLuint> shaders;
			map<string, GLuint> attributes;
			map<string, GLuint> uniforms;
			
		public:
			Program();
			virtual ~Program();
			
			bool getLinkStatus() const;
			bool getDeleteStatus() const;
			bool getValidateStatus() const;
			int getInfoLogLength() const;
			int getAttachedShaders() const;
			int getActiveAttributes() const;
			int getActiveAttributeMaxLength() const;
			int getActiveUniforms() const;
			int getActiveUniformMaxLength() const;
			string getInfoLog() const;
			
			unsigned int getId() const;
			bool loadVertexShaderFile( const string& filename );
			bool loadFragmentShaderFile( const string& filename );
			bool loadVertexShader( const string& source );
			bool loadFragmentShader( const string& source );
			
			bool attachShader( Shader * shader, bool deleteShader = false );
			bool link();
			
			void use();
	};
}

#endif

