#ifndef __OPENGL_SCREEN_H
#define	__OPENGL_SCREEN_H	1

#include <SDL2/SDL.h>
#include <opengl/Color.h>

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
			
			int width;
			int height;
			
			Color clearColor;
		
		public:
			static bool initialize( const char * title, int width = 800, int height = 600, bool resizable = false, int majorVersion = 2, int minorVersion = 1);
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

