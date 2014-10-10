#ifndef __OPENGL_OPENGL_H
#define __OPENGL_OPENGL_H	1

// OpenGL functions loading macros
#define DefineOpenGLFunction(type,func,...)	typedef type (APIENTRY * PFN_##func)(__VA_ARGS__); \
											extern PFN_##func func

#define InitializeOpenGLFunction(func)		PFN_##func func = 0;

#ifdef DEBUG0
#define LoadOpenGLFunction(func)			func = (PFN_##func) SDL_GL_GetProcAddress( #func ); \
											Logger::get() << "[OpenGL] Loaded " #func " at address " << (void*) func << "." << Logger::endl
#else
#define LoadOpenGLFunction(func)			func = (PFN_##func) SDL_GL_GetProcAddress( #func )
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#define DefineOpenGLFunction_Windows(type,func,...)	DefineOpenGLFunction(type,func,__VA_ARGS__)
#define InitializeOpenGLFunction_Windows(func)		InitializeOpenGLFunction(func)
#define LoadOpenGLFunction_Windows(func)			LoadOpenGLFunction(func)
#else
#define DefineOpenGLFunction_Windows(type,func,...) 
#define InitializeOpenGLFunction_Windows(func)				
#define LoadOpenGLFunction_Windows(func)			
#endif

#include <SDL2/SDL_opengl.h>
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
			static void initialize();
			static const string& getVendor();
			static const string& getRenderer();
			static const string& getVersion();
			static const set<string>& getExtensions();
			static bool hasExtension( const string& extension );
			static GLenum getError();
	};
}

// Draw functions
DefineOpenGLFunction_Windows( void, glDrawRangeElements, GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid * );

// Texture functions
DefineOpenGLFunction_Windows( void, glActiveTexture, GLenum );

// Buffer functions
DefineOpenGLFunction( void, glGenBuffers, GLsizei, GLuint * );
DefineOpenGLFunction( GLboolean, glIsBuffer, GLuint );
DefineOpenGLFunction( void, glDeleteBuffers, GLsizei, GLuint * );
DefineOpenGLFunction( void, glBindBuffer, GLenum, GLuint );
DefineOpenGLFunction( void, glBufferData, GLenum, GLsizeiptr, const void *, GLenum );

// Shaders functions
DefineOpenGLFunction( GLuint, glCreateShader, GLenum );
DefineOpenGLFunction( void, glDeleteShader, GLuint );
DefineOpenGLFunction( void, glGetShaderiv, GLuint, GLenum, GLint * );
DefineOpenGLFunction( void, glGetShaderInfoLog, GLuint, GLsizei, GLsizei *, GLchar * );
DefineOpenGLFunction( void, glShaderSource, GLuint, GLsizei, const GLchar **, const GLint * );
DefineOpenGLFunction( void, glCompileShader, GLuint );

// Programs functions
DefineOpenGLFunction( GLuint, glCreateProgram, void );
DefineOpenGLFunction( void, glDeleteProgram, GLuint );
DefineOpenGLFunction( void, glGetProgramiv, GLuint, GLenum, GLint * );
DefineOpenGLFunction( void, glGetProgramInfoLog, GLuint, GLsizei, GLsizei *, GLchar * );
DefineOpenGLFunction( void, glAttachShader, GLuint, GLuint );
DefineOpenGLFunction( void, glLinkProgram, GLuint );
DefineOpenGLFunction( void, glUseProgram, GLuint );
DefineOpenGLFunction( void, glGetActiveAttrib, GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar * );
DefineOpenGLFunction( void, glGetActiveUniform, GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar * );
DefineOpenGLFunction( void, glBindAttribLocation, GLuint, GLuint, const GLchar * );
DefineOpenGLFunction( void, glEnableVertexAttribArray, GLuint );
DefineOpenGLFunction( void, glDisableVertexAttribArray, GLuint );
DefineOpenGLFunction( void, glVertexAttribPointer, GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid * );
DefineOpenGLFunction( GLint, glGetAttribLocation, GLuint, const GLchar * );
DefineOpenGLFunction( GLint, glGetUniformLocation, GLuint, const GLchar * );

// Uniforms
DefineOpenGLFunction( void, glUniform1f, GLint, GLfloat );
DefineOpenGLFunction( void, glUniform2f, GLint, GLfloat, GLfloat );
DefineOpenGLFunction( void, glUniform3f, GLint, GLfloat, GLfloat, GLfloat );
DefineOpenGLFunction( void, glUniform4f, GLint, GLfloat, GLfloat, GLfloat, GLfloat );
DefineOpenGLFunction( void, glUniform1fv, GLint, GLsizei, const GLfloat * );
DefineOpenGLFunction( void, glUniform2fv, GLint, GLsizei, const GLfloat * );
DefineOpenGLFunction( void, glUniform3fv, GLint, GLsizei, const GLfloat * );
DefineOpenGLFunction( void, glUniform4fv, GLint, GLsizei, const GLfloat * );
DefineOpenGLFunction( void, glUniform1i, GLint, GLint );
DefineOpenGLFunction( void, glUniform2i, GLint, GLint, GLint );
DefineOpenGLFunction( void, glUniform3i, GLint, GLint, GLint, GLint );
DefineOpenGLFunction( void, glUniform4i, GLint, GLint, GLint, GLint, GLint );
DefineOpenGLFunction( void, glUniform1iv, GLint, GLsizei, const GLint * );
DefineOpenGLFunction( void, glUniform2iv, GLint, GLsizei, const GLint * );
DefineOpenGLFunction( void, glUniform3iv, GLint, GLsizei, const GLint * );
DefineOpenGLFunction( void, glUniform4iv, GLint, GLsizei, const GLint * );
DefineOpenGLFunction( void, glUniform1ui, GLint, GLuint );
DefineOpenGLFunction( void, glUniform2ui, GLint, GLuint, GLuint );
DefineOpenGLFunction( void, glUniform3ui, GLint, GLuint, GLuint, GLuint );
DefineOpenGLFunction( void, glUniform4ui, GLint, GLuint, GLuint, GLuint, GLuint );
DefineOpenGLFunction( void, glUniform1uiv, GLint, GLsizei, const GLuint * );
DefineOpenGLFunction( void, glUniform2uiv, GLint, GLsizei, const GLuint * );
DefineOpenGLFunction( void, glUniform3uiv, GLint, GLsizei, const GLuint * );
DefineOpenGLFunction( void, glUniform4uiv, GLint, GLsizei, const GLuint * );
DefineOpenGLFunction( void, glUniformMatrix4fv, GLint, GLsizei, GLboolean, const GLfloat * );

#endif
