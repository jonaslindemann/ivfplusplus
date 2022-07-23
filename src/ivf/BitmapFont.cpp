#include <ivf/BitmapFont.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <ivf/PolyState.h>
#include <ivf/PixelOps.h>
#include <ivfimage/PngImage.h>

// https://www.gamedev.net/forums/topic.asp?topic_id=330742

using namespace std;
using namespace ivf;

BitmapFont::BitmapFont(const std::string filename)
	:BillBoard(),
	m_filename{ filename },
	m_textureFilename{ "" },
	m_fontTexture{ nullptr },
	m_fontMaterial{ nullptr },
	m_textQuads{ nullptr },
	m_alignX{ ivf::HorizontalAlignment::Center },
	m_alignY{ ivf::VerticalAlignment::Middle },
	m_textSize{ 1.0 }
{
	string baseFilename = m_filename.substr(0, m_filename.find("."));
	m_textureFilename = baseFilename + "_0.png";

	this->load();
}

void ivf::BitmapFont::updateText()
{
	m_textQuads->clear();

	double currX = 0.0;
	auto totalHeight = m_charset.base;

	for (auto i = 0; i < m_text.size(); i++)
	{
		unsigned int id = m_text[i];
		auto x = m_charset.chars[m_text[i]].x;
		auto y = 512 - m_charset.chars[m_text[i]].y;
		auto width = m_charset.chars[m_text[i]].width;
		auto height = m_charset.chars[m_text[i]].height;
		float swidth = m_textSize * float(m_charset.chars[m_text[i]].width) / totalHeight;
		float sheight = m_textSize * float(m_charset.chars[m_text[i]].height) / totalHeight;
		float offsetX = m_textSize * float(m_charset.chars[m_text[i]].xOffset) / totalHeight;
		float offsetY = m_textSize * float(m_charset.chars[m_text[i]].yOffset) / totalHeight;
		float xAdvance = m_textSize * float(m_charset.chars[m_text[i]].xAdvance) / totalHeight;

		auto plane = ivf::QuadPlane::create();

		cout << offsetY << endl;

		plane->setCoord(0, currX + offsetX, -offsetY, 0.0);
		plane->setCoord(1, swidth + currX + offsetX, -offsetY, 0.0);
		plane->setCoord(2, swidth + currX + offsetX, -offsetY - sheight, 0.0);
		plane->setCoord(3, currX + offsetX, -offsetY - sheight, 0.0);

		plane->setTextureCoord(3, float(x) / float(m_charset.width), float(y - height) / float(m_charset.height));
		plane->setTextureCoord(2, float(x + width) / float(m_charset.width), float(y - height) / float(m_charset.height));
		plane->setTextureCoord(1, float(x + width) / float(m_charset.width), float(y) / float(m_charset.height));
		plane->setTextureCoord(0, float(x) / float(m_charset.width), float(y) / float(m_charset.height));

		m_textQuads->addChild(plane);
		currX += xAdvance;
	}

	m_textWidth = currX;
	m_textHeight = m_textSize;

	this->setAlignment(m_alignX, m_alignY);
}

void BitmapFont::setFilename(const std::string filename)
{
	m_filename = filename;
}

std::string BitmapFont::filename()
{
	return m_filename;
}

void ivf::BitmapFont::setText(const std::string text, float size)
{
	m_text = text;
	m_textSize = size;
	this->updateText();
}

const std::string ivf::BitmapFont::text()
{
	return m_text;
}

void ivf::BitmapFont::setSize(float size)
{
	m_textSize = size;
	this->updateText();
}

float ivf::BitmapFont::size()
{
	return m_textSize;
}

void ivf::BitmapFont::setAlignment(HorizontalAlignment alignX, VerticalAlignment alignY)
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

void BitmapFont::load()
{
	fstream f;
	
	f.open(m_filename, std::ios::in);

	if (!f)
	{
		cout << "Could not open " << m_filename << endl;
		return;
	}

	std::string line;
	std::string read, key, value;
	std::size_t i;
	
	while (!f.eof())
	{
		std::stringstream lineStream;
		std::getline(f, line);
		lineStream << line;

		//read the line's type
		lineStream >> read;
		if (read == "common")
		{
			//this holds common data
			while (!lineStream.eof())
			{
				std::stringstream converter;
				lineStream >> read;
				i = read.find('=');
				key = read.substr(0, i);
				value = read.substr(i + 1);

				//assign the correct value
				converter << value;
				if (key == "lineHeight")
					converter >> m_charset.lineHeight;
				else if (key == "base")
					converter >> m_charset.base;
				else if (key == "scaleW")
					converter >> m_charset.width;
				else if (key == "scaleH")
					converter >> m_charset.height;
				else if (key == "pages")
					converter >> m_charset.pages;
			}
		}
		else if (read == "char")
		{
			//this is data for a specific char
			unsigned short charId = 0;

			while (!lineStream.eof())
			{
				std::stringstream converter;
				lineStream >> read;
				i = read.find('=');
				key = read.substr(0, i);
				value = read.substr(i + 1);

				//assign the correct value
				converter << value;
				if (key == "id")
				{
					converter >> charId;
					cout << charId << endl;
				}
				else if (key == "x")
					converter >> m_charset.chars[charId].x;
				else if (key == "y")
					converter >> m_charset.chars[charId].y;
				else if (key == "width")
					converter >> m_charset.chars[charId].width;
				else if (key == "height")
					converter >> m_charset.chars[charId].height;
				else if (key == "xoffset")
					converter >> m_charset.chars[charId].xOffset;
				else if (key == "yoffset")
					converter >> m_charset.chars[charId].yOffset;
				else if (key == "xadvance")
					converter >> m_charset.chars[charId].xAdvance;
				else if (key == "page")
					converter >> m_charset.chars[charId].page;
			}
		}
	}

	// Load font texture

	auto fontImage = ivf::PngImage::create();
	fontImage->load(m_textureFilename);

	m_fontTexture = ivf::Texture::create();
	m_fontTexture->setFilters(GL_LINEAR, GL_LINEAR);
	m_fontTexture->setMode(GL_BLEND);
	m_fontTexture->setImage(fontImage);

	this->setTexture(m_fontTexture);

	// Set material

	m_fontMaterial = Material::create();
	m_fontMaterial->setDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_fontMaterial->setSpecularColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_fontMaterial->setAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);

	this->setMaterial(m_fontMaterial);

	m_textQuads = ivf::Composite::create();
	this->addChild(m_textQuads);
}

void ivf::BitmapFont::doPreGeometry()
{
	glPushAttrib(GL_BLEND | GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glAlphaFunc(GL_GEQUAL, 0.05);
	glEnable(GL_ALPHA_TEST);
}

void ivf::BitmapFont::doPostGeometry()
{
	glPopAttrib();
}
