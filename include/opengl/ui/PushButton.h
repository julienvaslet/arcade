#ifndef __OPENGL_UI_PUSHBUTTON_H
#define __OPENGL_UI_PUSHBUTTON_H	1

#include <opengl/ui/Button.h>

#include <vector>
using namespace std;

namespace opengl
{
	namespace ui
	{
		class PushButton : public Button
		{				
			protected:
				bool pushState;

			public:
				PushButton( const string& name, const string& value, bool pushed = false );
				virtual ~PushButton();
				
				void setPushState( bool state );
				bool getPushState() const;
			
				static bool eventMouseUp( Element * element );
				static bool eventMouseLeave( Element * element );
		};
	}
}

#endif

