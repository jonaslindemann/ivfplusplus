#include <ivf/BitmapFont.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <ivf/PolyState.h>
#include <ivf/PixelOps.h>
#include <ivfimage/PngImage.h>
#include <ivf/Texture.h>

// https://www.gamedev.net/forums/topic.asp?topic_id=330742

using namespace std;
using namespace ivf;

BitmapFont::BitmapFont(const std::string filename)
	:m_filename{ filename },
	m_textureFilename{ "" },
	m_fontTexture{ nullptr }
{
	string baseFilename = m_filename.substr(0, m_filename.find("."));
	m_textureFilename = baseFilename + "_0.png";
	this->load();
}

void BitmapFont::setFilename(const std::string filename)
{
	m_filename = filename;
}

std::string BitmapFont::filename()
{
	return m_filename;
}

Charset& ivf::BitmapFont::charset()
{
	return m_charset;
}

ivf::Texture* ivf::BitmapFont::texture()
{
	return m_fontTexture;
}

ivf::Material* ivf::BitmapFont::material()
{
	return m_fontMaterial;
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
	
	if (!fontImage->load(m_textureFilename))
		std::cout << "Could not load " << m_textureFilename << "\n";

	m_fontTexture = ivf::Texture::create();
	m_fontTexture->setFilters(GL_LINEAR, GL_LINEAR);
	m_fontTexture->setMode(GL_BLEND);
	m_fontTexture->setImage(fontImage);

	// Set material

	m_fontMaterial = Material::create();
	m_fontMaterial->setDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_fontMaterial->setSpecularColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_fontMaterial->setAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
}