#include "UILoader.h"
#include "ComponentManager.h"
#include "Theme.h"
#include "ButtonStyle.h"
#include "ImageStyleProperty.h"
#include "MenuStyle.h"
#include "FontStyleProperty.h"
#include "Label.h"
#include "Icon.h"
#include "MenuStack.h"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

using namespace Luxa;
/*
v3D::Vector3 to_vector3(const std::string & val)
{
	std::string var_x, var_y, var_z;
	size_t pos, pos2;
	pos = val.find(",");
	var_x = val.substr(0, pos);
	pos2 = val.find(",", pos+1);
	var_y = val.substr(pos+1, pos2);
	var_z = val.substr(pos2+1, val.length());

	v3D::Vector3 v;
	v[0] = boost::lexical_cast<float>(var_x);
	v[1] = boost::lexical_cast<float>(var_y);
	v[2] = boost::lexical_cast<float>(var_z);

	return v;
}
*/

v3D::Vector2 to_vector2(const std::string & val)
{
	v3D::Vector2 v;

	if (val.empty())
	{
		return v;
	}

	std::string var_x, var_y;
	size_t pos;
	pos = val.find(",");
	var_x = val.substr(0, pos);
	var_y = val.substr(pos+1, val.length());

	try
	{
		if (!var_x.empty())
		{
			v[0] = boost::lexical_cast<float>(var_x);
		}
		if (!var_y.empty())
		{
			v[1] = boost::lexical_cast<float>(var_y);
		}
	}
	catch (boost::bad_lexical_cast &)
	{
	}

	return v;
}


bool UILoader::load(const boost::property_tree::ptree & tree, ComponentManager * cm)
{
	// <vgui theme="default">
	//const PropertyTree & vgui = tree.find("vgui");
	const boost::property_tree::ptree & vgui = tree.get_child("vgui");
	// set default theme name
	std::string theme_name = vgui.get<std::string>("<xmlattr>.theme");
	cm->activeTheme(theme_name);
	// iterate over vgui child nodes
	BOOST_FOREACH(boost::property_tree::ptree::value_type const & v, tree.get_child("vgui"))
	{
        if(v.first == "theme")
		{
			loadTheme(v.second, cm);
		}
		else if (v.first == "font")
		{
			loadFont(v.second, cm);
		}
		else if (v.first == "button")
		{
			loadButton(v.second, cm);
		}
		else if (v.first == "label")
		{
			loadLabel(v.second, cm);
		}
		else if (v.first == "menu")
		{
			loadMenu(v.second, cm);
		}
		else if (v.first == "icon")
		{
			loadIcon(v.second, cm);
		}
	}

	// load all of the textures from any style image properties
	if (!loadTextures(cm))
	{
		return false;
	}

	return true;
}


bool UILoader::loadStyle(const boost::property_tree::ptree & style_node, boost::shared_ptr<Theme> the_theme)
{
	// <style class="button" name="default" state="normal">
	boost::shared_ptr<Style> the_style;
	std::string class_name, style_name;
	class_name = style_node.get<std::string>("<xmlattr>.class");
	style_name = style_node.get<std::string>("<xmlattr>.name");

	// derived style type attributes and instantiation
	if (class_name == "button")
	{
		std::string state_name = style_node.get<std::string>("<xmlattr>.state");
		Button::ButtonState state = Button::STATE_NORMAL;
		if (state_name == "hover")
		{
			state = Button::STATE_HOVER;
		}
		else if (state_name == "inactive")
		{
			state = Button::STATE_INACTIVE;
		}
		else if (state_name == "normal")
		{
			state = Button::STATE_NORMAL;
		}
		else if (state_name == "press")
		{
			state = Button::STATE_PRESS;
		}
		// else invalid state error

		boost::shared_ptr<ButtonStyle> button_style(new ButtonStyle(style_name, state));
		the_style = button_style;
	}
	else if (class_name == "menu")
	{
		boost::shared_ptr<MenuStyle> menu_style(new MenuStyle(style_name));
		the_style = menu_style;
	}
	else if (class_name == "label")
	{
		boost::shared_ptr<Style> label_style(new Style(style_name, "label"));
		the_style = label_style;
	}

	if (!the_style)
	{
		return false;
	}

	// load style properties
	BOOST_FOREACH(boost::property_tree::ptree::value_type const & v, style_node.get_child("image"))
	{
		if (!loadStyleProperty(v.second, the_style, "image"))
		{
			return false;
		}
	}
	BOOST_FOREACH(boost::property_tree::ptree::value_type const & v, style_node.get_child("font"))
	{
		if (!loadStyleProperty(v.second, the_style, "font"))
		{
			return false;
		}
	}

	// add style to theme
	the_theme->addStyle(the_style);

	return true;
}


bool UILoader::loadStyleProperty(const boost::property_tree::ptree & property_node, boost::shared_ptr<Style> the_style, const std::string & property_type)
{
	// align, name, source attributes
	// these three are common to both font and image styles
	std::string align, name, source;
	align = property_node.get<std::string>("<xmlattr>.align");
	name = property_node.get<std::string>("<xmlattr>.name");
	source = property_node.get<std::string>("<xmlattr>.source");
	StyleProperty::Alignment alignment = StyleProperty::NULL_ALIGNMENT;
	if (align == "top")
	{
		alignment = StyleProperty::TOP;
	}
	else if (align == "bottom")
	{
		alignment = StyleProperty::BOTTOM;
	}
	else if (align == "left")
	{
		alignment = StyleProperty::LEFT;
	}
	else if (align == "right")
	{
		alignment = StyleProperty::RIGHT;
	}

	// <image align="top-left" source="top-left.png" />
	// <font face="Courier" size="18" bold="true" italics="true" align="center" />
	if (property_type == "image")
	{
		/* texture loading happens in another independent pass so the texture naming won't be an issue 
			(full theme/style/state path name will be available)
		// this will break for button images with more than a single state
		std::string texture_name = the_style->name() + "-" + name;
		cm->textures()->load(source, texture_name);
		boost::shared_ptr<Hookah::Texture> texture = cm->textures()->texture(texture_name);
		*/
		boost::shared_ptr<StyleProperty> the_prop(new ImageStyleProperty(name, source));
		the_prop->align(alignment);
		the_style->addProperty(the_prop, "image");
	}
	else if (property_type == "font")
	{
		boost::shared_ptr<StyleProperty> the_prop(new FontStyleProperty(name, source));
		the_prop->align(alignment);
		the_style->addProperty(the_prop, "font");
	}

	// color style properties

	return true;
}


// <font face="Helvetica" size="32" bold="false" italics="false" name="ui-text-font" />
bool UILoader::loadFont(const boost::property_tree::ptree & font_node, ComponentManager * cm)
{
	boost::shared_ptr<v3D::FontCache> fc = cm->fonts();
	assert(fc != 0);
	std::string name, face;
	unsigned int size = 0;
	name = font_node.get<std::string>("<xmlattr>.name");
	face = font_node.get<std::string>("<xmlattr>.face");
	size = font_node.get<int>("<xmlattr>.size");
	if (!fc->load(name, face, size))
	{
		return false;
	}
	return true;
}

// <button label="Test Button" size="100., 50." position="100., 100." style="default" command="do_thing" scope="sample" />
bool UILoader::loadButton(const boost::property_tree::ptree & button_node, ComponentManager * cm)
{
	boost::shared_ptr<Button> button(new Button(cm));
	std::string label, command, scope;
	label = button_node.get<std::string>("<xmlattr>.label");
	command = button_node.get<std::string>("<xmlattr>.command");
	scope = button_node.get<std::string>("<xmlattr>.scope");
	v3D::Vector2 size = to_vector2(button_node.get<std::string>("<xmlattr>.size"));
	button->label(label);
	button->size(size);
	loadDefaultComponentAttributes(button_node, button);
	cm->addOverlayComponent(button);

	return true;
}

bool UILoader::loadLabel(const boost::property_tree::ptree & label_node, ComponentManager * cm)
{
	// <label label="Test Label" position="300., 300." />
	boost::shared_ptr<Label> label(new Label(cm));
	loadDefaultComponentAttributes(label_node, label);
	std::string str = label_node.get<std::string>("<xmlattr>.label");
	label->text(str);
	cm->addOverlayComponent(label);

	return true;
}

// default component attributes
void UILoader::loadDefaultComponentAttributes(const boost::property_tree::ptree & node, boost::shared_ptr<Component> component)
{
	component->position(to_vector2(node.get<std::string>("<xmlattr>.position")));
	component->style(node.get<std::string>("<xmlattr>.style"));
	component->name(node.get<std::string>("<xmlattr>.name"));

	std::string visible = node.get<std::string>("<xmlattr>.visible");
	if (!visible.empty())
	{
		bool val = false;
		if (visible == "true")
		{
			val = true;
		}
		component->visible(val);
	}
}

// <icon position="50., 200." name="test-icon" source="skins/test_icon.png" style="default" />
bool UILoader::loadIcon(const boost::property_tree::ptree & icon_node, ComponentManager * cm)
{
	std::string texture_name = icon_node.get<std::string>("<xmlattr>.name");
	std::string source = icon_node.get<std::string>("<xmlattr>.source");

	boost::shared_ptr<v3D::Texture> texture(new v3D::Texture(cm->loadImage(source)));
	cm->addTexture(texture_name, texture);

	boost::shared_ptr<Icon> icon(new Icon(texture, cm));

	loadDefaultComponentAttributes(icon_node, icon);
	cm->addOverlayComponent(icon);

	return true;
}

bool UILoader::loadMenu(const boost::property_tree::ptree & menu_node, ComponentManager * cm)
{
	boost::shared_ptr<MenuStack> menu(new MenuStack(cm));

	BOOST_FOREACH(boost::property_tree::ptree::value_type const & v, menu_node.get_child("item"))
	{
		loadMenuItem(cm, v.second, menu);
	}

	menu->active(0);
	menu->level(menu);

	loadDefaultComponentAttributes(menu_node, menu);
	cm->addOverlayComponent(menu);

	return true;
}

/*
	<menu>
		<item label="Options" command="submenu" type="submenu">
			<item label="Rounds: " param="maxScore" command="set_gamevar" scope="" type="numeric_input" />
			<item label="Player 1 Up: " param="leftPaddleUp" command="set_key" type="input" />
		</item>
		<item label="Singleplayer" command="setmode_SP" type="action" />
		<item label="Co-op" command="setmode_coop" type="action" />
		<item label="Multiplayer" command="setmode_MP" type="action" />
		<item label="Quit" command="quit" type="action" />
	</menu>
*/
void UILoader::loadMenuItem(ComponentManager * cm, const boost::property_tree::ptree & tree_item, boost::shared_ptr<Menu> parent)
{
	std::string label = tree_item.get<std::string>("<xmlattr>.label");
	std::string cmd = tree_item.get<std::string>("<xmlattr>.command");
	std::string scope = tree_item.get<std::string>("<xmlattr>.scope");
	std::string param = tree_item.get<std::string>("<xmlattr>.param");
	std::string type = tree_item.get<std::string>("<xmlattr>.type");
	boost::shared_ptr<MenuItem> item(new MenuItem(cm, type, label, cmd, scope, param));
	item->menu(parent);
	parent->addItem(item);

	// load menus (and their items) that are children of self
	boost::shared_ptr<Menu> cp(new Menu(cm));
	bool add_submenu = false;
	BOOST_FOREACH(boost::property_tree::ptree::value_type const & v, tree_item.get_child("item"))
	{
		// create a new menu that is a child menu of self and the parent menu of child
		loadMenuItem(cm, v.second, cp);
		add_submenu = true;
	}
	if (add_submenu)
	{
		cp->active(0);
		item->submenu(cp);
	}
}

bool UILoader::loadTheme(const boost::property_tree::ptree & theme_node, ComponentManager * cm)
{
	// <theme name="default">
	std::string theme_name = theme_node.get<std::string>("<xmlattr>.name");
	boost::shared_ptr<Theme> the_theme(new Theme(theme_name));

	// iterate theme child nodes
	BOOST_FOREACH(boost::property_tree::ptree::value_type const & v, theme_node.get_child("style"))
	{
		if (!loadStyle(v.second, the_theme))
		{
			return false;
		}
	}

	// add theme to component manager
	cm->addTheme(the_theme);
	return true;
}

bool UILoader::loadTextures(ComponentManager * cm)
{
	boost::shared_ptr<Theme> theme = cm->activeTheme();
	if (!theme)
		return false;

	// iterate over all the styles in the active theme
	std::vector< boost::shared_ptr<Style> > styles = theme->getStyleSet("", "");
	std::vector< boost::shared_ptr<Style> >::iterator style_iter = styles.begin();
	for (; style_iter != styles.end(); style_iter++)
	{
		// iterate over all the image style properties
		std::vector< boost::shared_ptr<StyleProperty> > properties = (*style_iter)->getPropertySet("", "image");
		std::vector< boost::shared_ptr<StyleProperty> >::iterator prop_iter = properties.begin();
		for (; prop_iter != properties.end(); prop_iter++)
		{
			boost::shared_ptr<ImageStyleProperty> image_prop = 
					boost::shared_dynamic_cast<ImageStyleProperty, StyleProperty>(*prop_iter);
			std::string texture_name = (*style_iter)->name() + "-" + image_prop->name() + "-" + image_prop->source();
			// load the image
			boost::shared_ptr<v3D::Image> img = cm->loadImage(image_prop->source());
			boost::shared_ptr<v3D::Texture> texture(new v3D::Texture(img));
			cm->addTexture(texture_name, texture);
			image_prop->texture(texture);
		}
	}

	return true;
}
