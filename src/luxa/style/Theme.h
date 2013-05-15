/**
 * (c) Joshua Farr <j.wgasa@gmail.com>
 */

#ifndef INCLUDED_LUXA_THEME
#define INCLUDED_LUXA_THEME

#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

namespace Luxa
{

	class Style;

	/**
	 * A vGUI theme.
	 * Themes consist of a name and a collection of styles.
	 */
	class Theme
	{
		public:
			Theme(const std::string & str);
			~Theme();

			/**
			 * Get the name of the theme.
			 * @return theme name
			 */
			std::string name() const;
			/**
			 * Add a style to the theme
			 * @param style the style to add
			 */
			void addStyle(boost::shared_ptr<Style> style);
			/**
			 * Get a set of styles with the matching name and class.
			 * If no name or class is specified then no filtering of that parameter will be done. This allows getting the
			 * subsets that consist of all styles, all of the same name, and all of the same class.
			 * @param name the name of the styles to find.
			 * @param class_name the class of the styles to get
			 * @return a collection of styles
			 */
			std::vector< boost::shared_ptr<Style> > getStyleSet(const std::string & name, const std::string & class_name) const;

		private:
			std::string name_;
			std::vector< boost::shared_ptr<Style> > styles_;
	};

}; // end namespace Luxa


#endif // INCLUDED_LUXA_THEME
