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
		
		Label::Label( const string& name, const string& value ) : Element(name), value(value), color(0.0f,0.0f,0.0f)
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
		
		void Label::prepareRendering( unsigned int ticks )
		{
			//Logger::get() << "[Label] Preparing rendering" << Logger::endl;
			Point2D origin( this->rectangle.getOrigin().getX(), this->rectangle.getOrigin().getY() );
			
			this->ui->getFont()->write( origin, this->value, this->color, 1.0f );
			
			Element::renderFunctions.insert( &Label::render );
		}
		
		void Label::render( unsigned int ticks )
		{
			//Logger::get() << "[Label] Rendering." << Logger::endl;
		}
	}
}
