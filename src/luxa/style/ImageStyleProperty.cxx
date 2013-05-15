/**
 * (c) Joshua Farr <j.wgasa@gmail.com>
 */

#include "ImageStyleProperty.h"

using namespace Luxa;

ImageStyleProperty::ImageStyleProperty(const std::string & name, const std::string & src) : StyleProperty(name), source_(src)
{
}

ImageStyleProperty::~ImageStyleProperty()
{
}

boost::shared_ptr<v3D::GLTexture> ImageStyleProperty::texture(void) const
{
	return texture_;
}

void ImageStyleProperty::texture(boost::shared_ptr<v3D::GLTexture> tex)
{
	texture_ = tex;
}

std::string ImageStyleProperty::source() const
{
	return source_;
}

