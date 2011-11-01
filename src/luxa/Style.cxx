#include "Style.h"
#include "StyleProperty.h"

using namespace Luxa;

Style::Style(const std::string & str, const std::string & class_name) : name_(str), className_(class_name)
{
}

Style::~Style()
{
}

std::string Style::name() const
{
	return name_;
}

std::string Style::className() const
{
	return className_;
}

void Style::className(const std::string & str)
{
	className_ = str;
}

boost::shared_ptr<StyleProperty> Style::property(const std::string & name, const std::string & class_name) const
{
	boost::shared_ptr<StyleProperty> prop;
	std::pair<std::string, std::string> key(name, class_name);
	std::map<std::pair<std::string, std::string>, boost::shared_ptr<StyleProperty> >::const_iterator iter = properties_.find(key);
	if (iter != properties_.end())
		prop = iter->second;
	return prop;
}

void Style::addProperty(boost::shared_ptr<StyleProperty> prop, const std::string & class_name)
{
	std::pair<std::string, std::string> key(prop->name(), class_name);
	properties_[key] = prop;
}

std::vector< boost::shared_ptr<StyleProperty>  > Style::getPropertySet(const std::string & name, const std::string & class_name) const
{
	std::map<std::pair<std::string, std::string>, boost::shared_ptr<StyleProperty> >::const_iterator iter = properties_.begin();
	std::vector< boost::shared_ptr<StyleProperty> > props;
	for (; iter != properties_.end(); iter++)
	{
		if ((name == "" || (*iter).first.first == name) && (class_name == "" || (*iter).first.second  == class_name))
			props.push_back((*iter).second);
	}
	return props;
}
