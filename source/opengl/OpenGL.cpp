#include <opengl/OpenGL.h>
#include <SDL2/SDL_opengl.h>

#include <sstream>

using namespace std;

namespace opengl
{
	string OpenGL::vendor;
	string OpenGL::renderer;
	string OpenGL::version;
	set<string> OpenGL::extensions;
	
	OpenGL::OpenGL()
	{
	}
	
	OpenGL::~OpenGL()
	{
	}
	
	const string& OpenGL::getVendor()
	{
		if( OpenGL::vendor.length() == 0 )
			OpenGL::vendor = (const char *) glGetString( GL_VENDOR );
			
		return OpenGL::vendor;
	}
	
	const string& OpenGL::getRenderer()
	{
		if( OpenGL::renderer.length() == 0 )
			OpenGL::renderer = (const char *) glGetString( GL_RENDERER );
			
		return OpenGL::renderer;
	}
	
	const string& OpenGL::getVersion()
	{
		if( OpenGL::version.length() == 0 )
			OpenGL::version = (const char *) glGetString( GL_VERSION );
			
		return OpenGL::version;
	}
	
	const set<string>& OpenGL::getExtensions()
	{
		if( OpenGL::extensions.size() == 0 )
		{
			const char * exts = (const char *) glGetString( GL_EXTENSIONS );
		
			unsigned int i = 0;
			stringstream extension;
		
			while( exts[i] != 0 )
			{
				if( exts[i] == ' ' )
				{
					OpenGL::extensions.insert( extension.str() );
					extension.str( "" );
				}
				else
					extension << exts[i];
			
				i++;
			}
		}
		
		return OpenGL::extensions;
	}
		
	bool OpenGL::hasExtension( const string& extension )
	{
		set<string>::const_iterator it = OpenGL::getExtensions().find( extension );
		return it != OpenGL::getExtensions().end();
	}
}
