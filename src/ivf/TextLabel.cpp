#include <ivf/TextLabel.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <ivf/PolyState.h>
#include <ivf/PixelOps.h>
#include <ivfimage/PngImage.h>

// https://www.gamedev.net/forums/topic.asp?topic_id=330742

using namespace std;
using namespace ivf;

TextLabel::TextLabel()
	:BillBoard(),
	m_font{ nullptr },
	m_textQuads{ nullptr },
	m_alignX{ ivf::HorizontalAlignment::Center },
	m_alignY{ ivf::VerticalAlignment::Middle },
	m_textSize{ 1.0 }
{
	m_textQuads = ivf::Composite::create();
	this->addChild(m_textQuads);
}

void ivf::TextLabel::updateText()
{
	if (m_font != nullptr)
	{
        m_textQuads->deleteAll();

		double currX = 0.0;
		auto totalHeight = m_font->charset().base;

		for (auto i = 0; i < m_text.size(); i++)
		{
			unsigned int id = m_text[i];
			auto x = m_font->charset().chars[m_text[i]].x;
			auto y = 512 - m_font->charset().chars[m_text[i]].y;
			auto width = m_font->charset().chars[m_text[i]].width;
			auto height = m_font->charset().chars[m_text[i]].height;
			float swidth = m_textSize * float(m_font->charset().chars[m_text[i]].width) / totalHeight;
			float sheight = m_textSize * float(m_font->charset().chars[m_text[i]].height) / totalHeight;
			float offsetX = m_textSize * float(m_font->charset().chars[m_text[i]].xOffset) / totalHeight;
			float offsetY = m_textSize * float(m_font->charset().chars[m_text[i]].yOffset) / totalHeight;
			float xAdvance = m_textSize * float(m_font->charset().chars[m_text[i]].xAdvance) / totalHeight;

			auto plane = ivf::QuadPlane::create();

			plane->setCoord(0, currX + offsetX, -offsetY, 0.0);
			plane->setCoord(1, swidth + currX + offsetX, -offsetY, 0.0);
			plane->setCoord(2, swidth + currX + offsetX, -offsetY - sheight, 0.0);
			plane->setCoord(3, currX + offsetX, -offsetY - sheight, 0.0);

			plane->setTextureCoord(3, float(x) / float(m_font->charset().width), float(y - height) / float(m_font->charset().height));
			plane->setTextureCoord(2, float(x + width) / float(m_font->charset().width), float(y - height) / float(m_font->charset().height));
			plane->setTextureCoord(1, float(x + width) / float(m_font->charset().width), float(y) / float(m_font->charset().height));
			plane->setTextureCoord(0, float(x) / float(m_font->charset().width), float(y) / float(m_font->charset().height));

			m_textQuads->addChild(plane);
			currX += xAdvance;
		}

		m_textWidth = currX;
		m_textHeight = m_textSize;

		this->setAlignment(m_alignX, m_alignY);

	}
}

void ivf::TextLabel::updateExistingText()
{
	if (m_font != nullptr)
	{
		double currX = 0.0;
		auto totalHeight = m_font->charset().base;

		for (auto i = 0; i < m_textQuads->getSize(); i++)
		{
			unsigned int id = m_text[i];
			auto x = m_font->charset().chars[m_text[i]].x;
			auto y = 512 - m_font->charset().chars[m_text[i]].y;
			auto width = m_font->charset().chars[m_text[i]].width;
			auto height = m_font->charset().chars[m_text[i]].height;
			float swidth = m_textSize * float(m_font->charset().chars[m_text[i]].width) / totalHeight;
			float sheight = m_textSize * float(m_font->charset().chars[m_text[i]].height) / totalHeight;
			float offsetX = m_textSize * float(m_font->charset().chars[m_text[i]].xOffset) / totalHeight;
			float offsetY = m_textSize * float(m_font->charset().chars[m_text[i]].yOffset) / totalHeight;
			float xAdvance = m_textSize * float(m_font->charset().chars[m_text[i]].xAdvance) / totalHeight;

			ivf::QuadPlane* plane = static_cast<ivf::QuadPlane*>(m_textQuads->getChild(i));

			plane->setCoord(0, currX + offsetX, -offsetY, 0.0);
			plane->setCoord(1, swidth + currX + offsetX, -offsetY, 0.0);
			plane->setCoord(2, swidth + currX + offsetX, -offsetY - sheight, 0.0);
			plane->setCoord(3, currX + offsetX, -offsetY - sheight, 0.0);

			plane->setTextureCoord(3, float(x) / float(m_font->charset().width), float(y - height) / float(m_font->charset().height));
			plane->setTextureCoord(2, float(x + width) / float(m_font->charset().width), float(y - height) / float(m_font->charset().height));
			plane->setTextureCoord(1, float(x + width) / float(m_font->charset().width), float(y) / float(m_font->charset().height));
			plane->setTextureCoord(0, float(x) / float(m_font->charset().width), float(y) / float(m_font->charset().height));

			currX += xAdvance;
		}

		m_textWidth = currX;
		m_textHeight = m_textSize;

		this->setAlignment(m_alignX, m_alignY);
	}
}

void ivf::TextLabel::setFont(ivf::BitmapFont* font)
{
	m_font = font;

	// Load font texture

	this->setTexture(m_font->texture());
	this->setMaterial(m_font->material());
}

ivf::BitmapFont* ivf::TextLabel::font()
{
	return m_font;
}

void ivf::TextLabel::setText(const std::string text, float size)
{
	m_text = text;
	m_textSize = size;
	this->updateText();
}

const std::string ivf::TextLabel::text()
{
	return m_text;
}

void ivf::TextLabel::setSize(float size)
{
	m_textSize = size;
	this->updateExistingText();
}

float ivf::TextLabel::size()
{
	return m_textSize;
}

void ivf::TextLabel::setAlignment(HorizontalAlignment alignX, VerticalAlignment alignY)
{
	m_alignX = alignX;
	m_alignY = alignY;

	if (m_textQuads != nullptr)
	{
		double dx, dy;

		if (m_alignX == HorizontalAlignment::Center)
			dx = -m_textWidth / 2.0;
		if (m_alignX == HorizontalAlignment::Left)
			dx = 0.0;
		if (m_alignX == HorizontalAlignment::Right)
			dx = -m_textWidth;

		if (m_alignY == VerticalAlignment::Middle)
			dy = -m_textHeight/2.0;
		if (m_alignY == VerticalAlignment::Bottom)
			dy = 0.0;
		if (m_alignY == VerticalAlignment::Top)
			dy = -m_textHeight;

		m_textQuads->setPosition(dx, dy+m_textHeight, 0.0);
	}
}

void ivf::TextLabel::doPreGeometry()
{
	glPushAttrib(GL_BLEND | GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glAlphaFunc(GL_GEQUAL, 0.05);
	glEnable(GL_ALPHA_TEST);
}

void ivf::TextLabel::doPostGeometry()
{
	glPopAttrib();
}
