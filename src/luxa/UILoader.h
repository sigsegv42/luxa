#ifndef INCLUDED_LUXA_UILOADER
#define INCLUDED_LUXA_UILOADER

#include <util/PropertyTree.h>
#include <boost/shared_ptr.hpp>

namespace Luxa
{
	// forward declarations
	class ComponentManager;
	class Component;
	class Menu;
	class MenuItem;
	class Theme;
	class Style;

	/**
	 * Load the ui from a property tree description.
	 */
	class UILoader
	{
		public:
			/**
			 * Load the ui from a property tree
			 * @param tree the property tree to load from
			 * @param cm a pointer to the component manager to be used
			 * @return whether the configuration was successfully loaded or not
			 */
			bool load(const PropertyTree & tree, ComponentManager * cm);

		protected:
			bool loadTheme(const PropertyTree & theme_node, ComponentManager * cm);
			bool loadStyle(const PropertyTree & style_node, boost::shared_ptr<Theme> the_theme);
			bool loadStyleProperty(const PropertyTree & property_node, boost::shared_ptr<Style> the_style);

			// load all of the textures specified within image style properties
			bool loadTextures(ComponentManager * cm);

			bool loadMenu(const PropertyTree & menu_node, ComponentManager * cm);
			void loadMenuItem(ComponentManager * cm, const PropertyTree & tree_item, boost::shared_ptr<Menu> parent);
			bool loadLabel(const PropertyTree & label_node, ComponentManager * cm);
			bool loadButton(const PropertyTree & button_node, ComponentManager * cm);
			bool loadFont(const PropertyTree & font_node, ComponentManager * cm);
			bool loadIcon(const PropertyTree & icon_node, ComponentManager * cm);
			void loadDefaultComponentAttributes(const PropertyTree & node, boost::shared_ptr<Component> component);
	};

}; // end namespace Luxa

#endif // INCLUDED_LUXA_UILOADER
