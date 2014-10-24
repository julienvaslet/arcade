#ifndef __OPENGL_BITMAPFONT_H
#define __OPENGL_BITMAPFONT_H	1

#include <opengl/Font.h>
#include <opengl/Program.h>
#include <opengl/Texture2D.h>
#include <opengl/ArrayBufferObject.h>
#include <opengl/ElementArrayBufferObject.h>

using namespace std;

namespace opengl
{
	class BitmapFont : public Font
	{
		protected:
			static unsigned int instances;
			static Program * program;
		
			ArrayBufferObject * vertices;
			ArrayBufferObject * textureCoordinates;
			ElementArrayBufferObject * indices;
			Texture2D * texture;
			unsigned int charactersByLine;
			unsigned int characterWidth;
			unsigned int characterHeight;
			unsigned int marginWidth;
			unsigned int marginHeight;
			float relativeCharacterWidth;
			float relativeCharacterHeight;
			
		public:
			BitmapFont( const string& filename, unsigned int characterWidth, unsigned int characterHeight, unsigned int marginWidth = 0, unsigned int marginHeight = 0 );
			virtual ~BitmapFont();
		
			virtual void render( const Point2D& origin, const string& text, const Color& color, float size = 0.0f ) const;
			virtual void renderSize( Point2D& origin, const string& text, float size = 0.0f ) const;
			virtual unsigned int renderWidth( const string& text, float size = 0.0f ) const;
			virtual unsigned int renderHeight( const string& text, float size = 0.0f ) const;
			
			static string getFontNameFromPath( const string& filename );
	};
}

#endif
