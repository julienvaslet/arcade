#include <opengl/ui/Label.h>

using namespace std;

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace opengl
{
	namespace ui
	{
		//bool Label::useRenderingCache = true;
		
		Label::Label( const string& name, const string& value ) : Element(name), value(value), color(1.0f,1.0f,1.0f)
		{
			#ifdef DEBUG0
			Logger::get() << "[Label#" << this->name <<"] Created." << Logger::endl;
			#endif
		}
		
		Label::~Label()
		{
			#ifdef DEBUG0
			Logger::get() << "[Label#" << this->name << "] Deleted." << Logger::endl;
			#endif
		}
		
		const string& Label::getValue() const
		{
			return this->value;
		}
		
		void Label::setValue( const string& value, bool resize )
		{
			this->value = value;
			
			if( resize )
				this->autoResize();
		}
		
		void Label::setColor( const string& color )
		{
			this->color.setColor( color );
		}
		
		void Label::setColor( const Color& color )
		{
			this->color.setColor( color );
		}
		
		const Color& Label::getColor() const
		{
			return this->color;
		}
		
		void Label::autoResize()
		{
			Point2D size;
			
			if( this->ui != NULL )
			{
				Font * font = this->ui->getFont();
				
				if( font != NULL )
					font->getTextSize( size, this->value, this->ui->getFontSize() );
			}
			
			this->rectangle.resize( size.getX() + OPENGL_UI_LABEL_HORIZONTAL_PADDING * 2, size.getY() + OPENGL_UI_LABEL_VERTICAL_PADDING * 2 );
		}
		
		void Label::prepareRendering( unsigned int ticks )
		{
			//Logger::get() << "[Label] Preparing rendering" << Logger::endl;
			Point2D origin( this->rectangle.getOrigin().getX() + OPENGL_UI_LABEL_HORIZONTAL_PADDING, this->rectangle.getOrigin().getY() + OPENGL_UI_LABEL_VERTICAL_PADDING );
			
			this->ui->getFont()->write( origin, this->value, this->color, this->ui->getFontSize() );
			
			//Element::renderFunctions.insert( &Label::render );
		}
		
		void Label::render( unsigned int ticks )
		{
			//Logger::get() << "[Label] Rendering." << Logger::endl;
		}
	}
}
