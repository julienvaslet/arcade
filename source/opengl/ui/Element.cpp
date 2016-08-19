#include <opengl/ui/Element.h>
#include <opengl/ui/UserInterface.h>

#include <vector>
using namespace std;

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace opengl
{
	namespace ui
	{
		set<ElementRenderFunction> Element::renderFunctions;
		
		Element::Element( const string& name ) : ui(NULL), name(name)
		{
		}
		
		Element::~Element()
		{
		}
		
		const string& Element::getName() const
		{
			return this->name;
		}
		
		void Element::setUserInterface( UserInterface * ui )
		{
			this->ui = ui;
		}
		
		void Element::render( unsigned int ticks )
		{
			for( set<ElementRenderFunction>::iterator it = Element::renderFunctions.begin() ; it != Element::renderFunctions.end() ; it++ )
				(*it)( ticks );
				
			Element::renderFunctions.clear();
		}
	}
}
