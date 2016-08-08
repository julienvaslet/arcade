#ifndef __OPENGL_UI_ELEMENT_CONTAINER_H
#define __OPENGL_UI_ELEMENT_CONTAINER_H	1

#include <opengl/ui/Element.h>

#include <vector>
using namespace std;

namespace opengl
{
	namespace ui
	{
		class ElementContainer
		{
			protected:
				unsigned int currentIndex;
				vector<Element *> elements;
			
			public:
				ElementContainer();
				virtual ~ElementContainer();
		};
	}
}

#endif

