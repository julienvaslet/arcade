#ifndef __OPENGL_FONT_H
#define __OPENGL_FONT_H	1

#include <opengl/OpenGL.h>
#include <opengl/Point2D.h>

#include <map>

using namespace std;

namespace opengl
{
	class Font
	{
		protected:
			static map<string, Font *> fonts;

			Font( const string& name );
			virtual ~Font();
			
		public:
			virtual void render( const Point2D& origin, const string& text, float size = 0.0f ) const = 0;
			virtual void renderSize( Point2D& origin, const string& text, float size = 0.0f ) const = 0;
			virtual unsigned int renderWidth( const string& text, float size = 0.0f ) const = 0;
			virtual unsigned int renderHeight( const string& text, float size = 0.0f ) const = 0;
			
			static Font * get( const string& name );
			static void destroy( const string& name );
			static void destroy();
	};
}

#endif
