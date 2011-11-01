#ifndef INCLUDED_LUXA_COMPONENTRENDERER
#define INCLUDED_LUXA_COMPONENTRENDERER

#include <3dtypes/Vector2.h>
#include <image/Texture.h>
#include <font/FontCache.h>

#include <boost/shared_ptr.hpp>

#include "Overlay.h"
#include "Theme.h"

namespace Luxa
{

	/**
	 * A class for handling all of the actual OpenGL vGUI rendering.
	 */
	class ComponentRenderer
	{
		public:
			ComponentRenderer();
			ComponentRenderer(boost::shared_ptr<v3D::FontCache> fc);
			~ComponentRenderer();

			void overlay(const Overlay & o);

			/**
			 * Draw the ui
			 */
			void draw();
			/**
			 * Resize the rendering area
			 * @param width the new width
			 * @param height the new height
			 */
			void resize(int width, int height);
			/**
			 * Draw a texture mapped quad
			 * @param texture the texture to map on the quad
			 * @param position_x the x coordinate of the upper left corner of the quad on the screen
			 * @param position_y the y coordinate of the upper left corner of the quad on the screen
			 * @param width the width of the quad in pixels
			 * @param height the height of the quad in pixels
			 */
			bool drawTexturedQuad(boost::shared_ptr<v3D::Texture> texture, float position_x, float position_y, float width, float height);
			/**
			 * Draw a texture
			 * @param texture the texture to draw
			 * @param position the position to draw the texture at
			 */
			bool drawTexture(boost::shared_ptr<v3D::Texture> texture, v3D::Vector2 position);

			/**
			 * Set the rendering color
			 * @param c the color to use for rendering
			 */
			void color(const v3D::Color3 & c);

			/**
			 * Translate to the requested position relative to the current position
			 * @param pos the new relative position to move to
			 */
			void position(const v3D::Vector2 & pos);
			/**
			 * Push the rendering matrix state
			 */
			void push();
			/**
			 * Pop the rendering matrix state
			 */
			void pop();
			/**
			 * Clear the rendering matrix state
			 */
			void clear();
			/**
			 * Get the width of the managed component area.
			 * This is usually the same as the window canvas width
			 * @return the current width
			 */
			unsigned int width() const;
			/**
			 * Get the height of the managed component area.
			 * This is usually the same as the window canvas height
			 * @return the current height
			 */
			unsigned int height() const;

			void prepare();
			void post();
			/**
			 * Get the font cache used by the vgui system.
			 * @return a pointer to the font cache
			 */
			boost::shared_ptr<v3D::FontCache> fonts() const;
			/**
			 * Get the default font for a component class.
			 * @param style_class the class type of the component
			 * @param theme the active theeme
			 * @return the font for the requested component type
			 */
			boost::shared_ptr<v3D::Font2D> getDefaultFont(const std::string & style_class, boost::shared_ptr<Theme> theme);

		private:
			int width_;
			int height_;
			Overlay overlay_;
			boost::shared_ptr<v3D::FontCache> fonts_;
	};

}; // end namespace Luxa

#endif // INCLUDED_LUXA_COMPONENTRENDERER
