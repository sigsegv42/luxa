#include "Menu.h"
#include "ComponentManager.h"

using namespace Luxa;

Menu::Menu(ComponentManager * cm) : Component(cm)
{
}

void Menu::parent(boost::weak_ptr<Menu> p)
{
	parent_ = p;
}

boost::shared_ptr<Menu> Menu::level() const
{
	return level_.lock();
}

void Menu::level(boost::weak_ptr<Menu> m)
{
	level_ = m;
}

void Menu::addItem(boost::shared_ptr<MenuItem> item)
{
	items_.push_back(item);
}

boost::shared_ptr<MenuItem> Menu::active() const
{
	boost::shared_ptr<MenuItem> item;
	if (active_ != items_.end())
		item = *active_;
	return item;
}

void Menu::active(int idx)
{
	if (idx < 0 || static_cast<unsigned>(idx) >= items_.size())
		active_ = items_.end();

	std::vector< boost::shared_ptr<MenuItem> >::iterator iter = items_.begin();
	for (int i = 0; i < idx; i++)
	{
		iter++;
	}
	active_ = iter;
}

bool Menu::next()
{
	boost::shared_ptr<Menu> lvl = level_.lock();
	lvl->active_++;
	if (lvl->active_ == lvl->items_.end()) // wrap around
	{
		lvl->active_ = lvl->items_.begin();
		return false;
	}
	return true;
}

bool Menu::previous()
{
	boost::shared_ptr<Menu> lvl = level_.lock();
	if (lvl->active_ == lvl->items_.end()) // wrap around
	{
		lvl->active_ = lvl->items_.begin();
		return false;
	}
	if (lvl->active_ != lvl->items_.begin())
		lvl->active_--;
	else
	{
		lvl->active_ = lvl->items_.end();
		lvl->active_--;
	}
	return true;
}

bool Menu::up()
{
	boost::shared_ptr<Menu> lvl = level_.lock();
	if (lvl->parent_.expired())
	{
		return false;
	}
	level_ = lvl->parent_;
	return true;
}

bool Menu::down()
{
	if (active_ == items_.end())
	{
		return false;
	}
	boost::shared_ptr<Menu> sm = (*active_)->submenu();
	if (sm)
	{
		level_ = sm;
		return true;
	}
	return false;
}

size_t Menu::size() const
{
	return items_.size();
}

boost::shared_ptr<MenuItem> & Menu::operator[](unsigned int i)
{
	assert(i < items_.size());
	return items_[i];
}

void Menu::activate()
{
	boost::shared_ptr<MenuItem> item = active();
	if (item)
	{
		if (item->type() == "submenu" && item->submenu()) // menu item has a submenu so activate the submenu
		{
			bool activated = down();
		}
		else if (item->type() == "action") // menu item represents a command so execute the bound command
		{
			manager_->execCommand(item->command(), item->scope(), item->param());
		}
		else if (item->type() == "input" || item->type() == "numeric_input" || item->type() == "key_input")
		{
			// the ui needs to capture all input until the next ui activation (e.g. another select menu command bound
			// event is received)
		}
	}
}
