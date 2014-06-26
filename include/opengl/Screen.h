#ifndef __OPENGL_SCREEN_H
#define	__OPENGL_SCREEN_H	1

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
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
		
		public:
			static bool initialize( const char * title, int width = 800, int height = 600, bool resizable = false );
			static Screen * get();
			static void destroy();
		
			SDL_GLContext getContext();
			void resetRenderColor();
			void setRenderColor( const Color& color );
			void render();
			void clear();
			int getWidth();
			int getHeight();
	};
}

#endif

