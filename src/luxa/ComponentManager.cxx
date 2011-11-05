#include "ComponentManager.h"
#include "Component.h"
#include "Theme.h"

#include <vertical3d/image/ImageFactory.h>

#include <boost/bind.hpp>
#include <boost/cast.hpp>

using namespace Luxa;

ComponentManager::ComponentManager(boost::shared_ptr<v3D::FontCache> fc, boost::shared_ptr<v3D::CommandDirectory> directory) : renderer_(fc), directory_(directory)
{
}

ComponentRenderer * ComponentManager::renderer()
{
	return &renderer_;
}

void ComponentManager::addTexture(const std::string & name, const boost::shared_ptr<v3D::Texture> & texture)
{
	textures_[name] = texture;
}

boost::shared_ptr<v3D::Image> ComponentManager::loadImage(const std::string & filename)
{
	v3D::ImageFactory factory;
	boost::shared_ptr<v3D::Image> img = factory.read(filename);
	return img;
}

void ComponentManager::motion(unsigned int x, unsigned int y)
{
	// ignore motion outside managed area
	if (x > renderer_.width() || y > renderer_.height())
		return;

	// get the old component that existed at the last mouse position
	boost::shared_ptr<Component> old_component = intersect(mouse_);

	// store mouse position (so it can be used by mouse button events)
	mouse_[0] = boost::numeric_cast<float>(x);
	mouse_[1] = boost::numeric_cast<float>(y);

	// get the control at the top of the stack at these coordinates
	boost::shared_ptr<Component> component = intersect(mouse_);

	// trigger mouse out event on old component
	if (old_component && (component != old_component))
	{
		v3D::EventInfo leave_event("leave", v3D::EventInfo::MATCH_STATE_OFF);
		old_component->notify(leave_event);
	}

	// trigger mouse in event on component
	if (component && (component != old_component))
	{
		v3D::EventInfo enter_event("enter", v3D::EventInfo::MATCH_STATE_ON);
		component->notify(enter_event);
	}
}

boost::shared_ptr<Component> ComponentManager::intersect(v3D::Vector2 point) const
{
	boost::shared_ptr<Component> component;
	std::vector< boost::shared_ptr<Component> >::const_iterator iter = components_.begin();
	for (; iter != components_.end(); iter++)
	{
		if ((*iter)->bound().intersect(point) && 
			(component && component->depth() < (*iter)->depth()))
		{
			component = (*iter);
		}
	}
	return component;
}

void ComponentManager::buttonPressed(unsigned int button)
{
	// get the control at the top of the stack at these coordinates
	boost::shared_ptr<Component> component = intersect(mouse_);
	v3D::EventInfo e("button_pressed", v3D::EventInfo::MATCH_STATE_ON);
	component->notify(e);
}

void ComponentManager::buttonReleased(unsigned int button)
{
	// get the control at the top of the stack at these coordinates
	boost::shared_ptr<Component> component = intersect(mouse_);
	v3D::EventInfo e("button_released", v3D::EventInfo::MATCH_STATE_OFF);
	component->notify(e);
}

void ComponentManager::keyPressed(const std::string & key)
{
}

void ComponentManager::keyReleased(const std::string & key)
{
}

void ComponentManager::activeTheme(const std::string & name)
{
	activeTheme_ = name;
}

boost::shared_ptr<Component> ComponentManager::getComponent(const std::string & name) const
{
	std::vector< boost::shared_ptr<Component> >::const_iterator iter = components_.begin();
	for (; iter != components_.end(); iter++)
	{
		if ((*iter)->name() == name)
			return (*iter);
	}
	boost::shared_ptr<Component> c;
	return c;
}

bool ComponentManager::toggleComponentVisibility(const std::string & name)
{
	boost::shared_ptr<Component> component = getComponent(name);
	bool vis = component->visible();
	if (component)
		component->visible(!vis);
	return !vis;
}


void ComponentManager::draw(Hookah::Window * window)
{
	renderer_.prepare();
	renderer_.draw();

	std::vector< boost::shared_ptr<Component> >::iterator iter = components_.begin();
	for (; iter != components_.end(); iter++)
	{
		if ((*iter)->visible())
		{
			(*iter)->draw(&renderer_, activeTheme());
		}
	}

	renderer_.post();
}

void ComponentManager::tick(unsigned int delta)
{

}

void ComponentManager::resize(int width, int height)
{
	renderer_.resize(width, height);
}

void ComponentManager::addOverlayComponent(boost::shared_ptr<Component> component)
{
	components_.push_back(component);
}


void ComponentManager::addTheme(boost::shared_ptr<Theme> theme)
{
	themes_[theme->name()] = theme;
}


unsigned int ComponentManager::width() const
{
	return renderer_.width();
}

unsigned int ComponentManager::height() const
{
	return renderer_.height();
}

boost::shared_ptr<v3D::FontCache> ComponentManager::fonts() const
{
	return renderer_.fonts();
}

boost::shared_ptr<Theme> ComponentManager::activeTheme() const
{
	boost::shared_ptr<Theme> theme;
	std::map<std::string, boost::shared_ptr<Theme> >::const_iterator iter = themes_.find(activeTheme_);
	if (iter != themes_.end())
		theme = iter->second;
	return theme;
}
