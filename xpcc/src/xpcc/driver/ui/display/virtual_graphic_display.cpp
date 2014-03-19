#include "virtual_graphic_display.hpp"

xpcc::VirtualGraphicDisplay::VirtualGraphicDisplay(xpcc::GraphicDisplay* display,
		xpcc::glcd::Point leftUpper, xpcc::glcd::Point rightLower):
		display(display), leftUpper(leftUpper), rightLower(rightLower),
		width(static_cast<uint16_t>(this->rightLower[0] - this->leftUpper[0])),
		height(static_cast<uint16_t>(this->rightLower[1] - this->leftUpper[1]))
{
}

void
xpcc::VirtualGraphicDisplay::setDisplay(xpcc::GraphicDisplay* display)
{
	this->display = display;
	return;
}

void
xpcc::VirtualGraphicDisplay::clear()
{
	//TODO switch black , white
	this->display->setColor(glcd::Color::black());
	this->display->fillRectangle(this->leftUpper, width, height);
	this->display->setColor(glcd::Color::white());
}

void
xpcc::VirtualGraphicDisplay::update()
{
	this->display->update();
	return;
}

void
xpcc::VirtualGraphicDisplay::setPixel(int16_t x, int16_t y)
{
	this->display->setPixel(x + this->leftUpper[0], y + this->leftUpper[1]);
}

void
xpcc::VirtualGraphicDisplay::clearPixel(int16_t x, int16_t y)
{
	this->display->clearPixel(x + this->leftUpper[0], y + this->leftUpper[1] );
}

bool
xpcc::VirtualGraphicDisplay::getPixel(int16_t x, int16_t y)
{
	return this->display->getPixel(x + this->leftUpper[0], y + this->leftUpper[1] );
}
