#ifndef _GRAPHICS_SCREEN_H
#define	_GRAPHICS_SCREEN_H	1

#include <SDL2/SDL.h>
#include <opengl/Color.h>

namespace opengl
{
	class Screen
	{
		protected:
			static Screen * instance;
		
			SDL_Window * window;
			SDL_Renderer * renderer;
		
			Screen();
			~Screen();
		
		public:
			static bool initialize( const char * title, int width = 800, int height = 600, bool resizable = false );
			static Screen * get();
			static void destroy();
		
			SDL_Renderer * getRenderer();
			void resetRenderColor();
			void setRenderColor( const Color& color );
			void render();
			void clear();
			int getWidth();
			int getHeight();
	};
}

#endif

