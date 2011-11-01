#ifndef INCLUDED_LUXA_COLORSTYLEPROPERTY
#define INCLUDED_LUXA_COLORSTYLEPROPERTY

#include "StyleProperty.h"

#include <3dtypes/Color3.h>

namespace Luxa
{

	/**
	 * A vGUI style property that defines a single color.
	 */
	class ColorStyleProperty : public StyleProperty
	{
		public:
			ColorStyleProperty();
			~ColorStyleProperty();

		private:
			v3D::Color3 color_;
	};

};

#endif // INCLUDED_LUXA_COLORSTYLEPROPERTY
