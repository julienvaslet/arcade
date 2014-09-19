#ifndef __OPENGL_OPENGL_H
#define __OPENGL_OPENGL_H	1

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <string>
#include <set>
using namespace std;

namespace opengl
{
	class OpenGL
	{
		protected:
			static string vendor;
			static string renderer;
			static string version;
			static set<string> extensions;
		
			OpenGL();
			~OpenGL();
			
		public:
			static const string& getVendor();
			static const string& getRenderer();
			static const string& getVersion();
			static const set<string>& getExtensions();
			static bool hasExtension( const string& extension );
			static GLenum getError();
	};
}

#endif
