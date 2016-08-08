#ifndef __OPENGL_UI_ELEMENT_H
#define __OPENGL_UI_ELEMENT_H	1

#define OPENGL_UI_MAX_ELEMENTS	65535

#include <map>
using namespace std;

namespace opengl
{
	namespace ui
	{
		class Element
		{
			protected:
				static map<unsigned int, Element *> elements;
				
				unsigned int id;
			
			public:
				Element();
				virtual ~Element();
				
				//how to render / prepare rendering with different types?
				
				static Element * get( unsigned int id );
				static void regenerateIds();
		};
	}
}

#endif

