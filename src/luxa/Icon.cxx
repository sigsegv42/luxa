/**
 * (c) Joshua Farr <j.wgasa@gmail.com>
 */

#include "Icon.h"
#include "ComponentRenderer.h"

using namespace Luxa;

Icon::Icon(boost::shared_ptr<v3D::GLTexture> texture, ComponentManager * cm) : Component(cm), texture_(texture)
{
}

Icon::~Icon()
{
}

void Icon::draw(ComponentRenderer * renderer, const boost::shared_ptr<Theme> & theme) const
{
	glm::vec2 pos = position();
	float width = static_cast<float>(texture_->width());
	float height = static_cast<float>(texture_->height());
	renderer->drawTexture(texture_, pos);
}

