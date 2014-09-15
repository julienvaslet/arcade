#ifndef __OPENGL_PROGRAM_H
#define __OPENGL_PROGRAM_H	1

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>

#include <string>
#include <map>

using namespace std;

namespace opengl
{
	class Program
	{
		protected:
			GLuint id;
			map<string, GLuint> variables;
			
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
			bool link();
			
			void use();
	};
}

#endif

