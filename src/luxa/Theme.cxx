#include "Theme.h"
#include "Style.h"

using namespace Luxa;

Theme::Theme(const std::string & str) : name_(str)
{
}

Theme::~Theme()
{
}

std::string Theme::name() const
{
	return name_;
}

void Theme::addStyle(boost::shared_ptr<Style> style)
{
	styles_.push_back(style);
}


std::vector< boost::shared_ptr<Style> > Theme::getStyleSet(const std::string & name, const std::string & class_name) const
{
	std::vector< boost::shared_ptr<Style> > matching_styles;
	std::vector< boost::shared_ptr<Style> >::const_iterator iter = styles_.begin();
	for (; iter != styles_.end(); iter++)
	{
		if ((name == "" || (*iter)->name() == name) && (class_name == "" || (*iter)->className() == class_name))
			matching_styles.push_back((*iter));
	}
	return matching_styles;
}
