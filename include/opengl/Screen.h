#ifndef __OPENGL_SCREEN_H
#define	__OPENGL_SCREEN_H	1

#include <SDL2/SDL.h>
#include <opengl/Color.h>

#ifdef __PI__
#define OPENGL_DEFAULT_MAJOR_VERSION	2
#define OPENGL_DEFAULT_MINOR_VERSION	0
#else
#define OPENGL_DEFAULT_MAJOR_VERSION	3
#define OPENGL_DEFAULT_MINOR_VERSION	0
#endif

namespace opengl
{
	class Screen
	{
		protected:
			static Screen * instance;
		
			SDL_Window * window;
			SDL_GLContext context;
		
			Screen();
			~Screen();
			
			int x;
			int y;
			int width;
			int height;
			
			Color clearColor;
		
		public:
			static bool initialize( int width = 800, int height = 600, int majorVersion = OPENGL_DEFAULT_MAJOR_VERSION, int minorVersion = OPENGL_DEFAULT_MINOR_VERSION );
			static Screen * get();
			static void destroy();
		
			SDL_GLContext getContext();
			void setClearColor( const Color& color );
			void render();
			void clear();
			int getWidth();
			int getHeight();
	};
}

#endif

