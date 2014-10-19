#ifndef __OPENGL_BITMAPFONT_H
#define __OPENGL_BITMAPFONT_H	1

#include <opengl/Font.h>
#include <opengl/Program.h>
#include <opengl/Texture2D.h>

using namespace std;

namespace opengl
{
	class BitmapFont : public Font
	{
		protected:
			static unsigned int instances;
			static Program * program;
		
			Texture2D * texture;
			unsigned int charactersByLine;
			unsigned int characterWidth;
			unsigned int characterHeight;
			float relativeCharacterWidth;
			float relativeCharacterHeight;
			
		public:
			BitmapFont( const string& filename, unsigned int characterWidth, unsigned int characterHeight );
			virtual ~BitmapFont();
		
			virtual void render( const Point2D& origin, const string& text ) const;
			virtual void renderSize( Point2D& origin, const string& text ) const;
			virtual unsigned int renderWidth( const string& text ) const;
			virtual unsigned int renderHeight( const string& text ) const;
	};
}

#endif
