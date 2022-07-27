#pragma once

#include <string>

#include <ivf/Billboard.h>
#include <ivf/Composite.h>
#include <ivf/QuadPlane.h>
#include <ivf/Texture.h>

namespace ivf {

struct CharDescriptor
{
	//clean 16 bytes
	unsigned short x, y;
	unsigned short width, height;
	float xOffset, yOffset;
	float xAdvance;
	unsigned short page;

	CharDescriptor() : x(0), y(0), width(0), height(0), xOffset(0), yOffset(0),
		xAdvance(0), page(0)
	{ }
};

struct Charset
{
	unsigned short lineHeight;
	unsigned short base;
	unsigned short width, height;
	unsigned short pages;
	CharDescriptor chars[256];
};

IvfSmartPointer(BitmapFont);

class BitmapFont : public ivf::Base {
private:
	Charset m_charset;
	std::string m_filename;
	std::string m_textureFilename;
	ivf::TexturePtr m_fontTexture;
	ivf::MaterialPtr m_fontMaterial;
public:
	BitmapFont(const std::string filename);

	IvfClassInfo("BitmapFont", Base);

	static BitmapFontPtr create(const std::string filename) { return BitmapFontPtr(new BitmapFont(filename)); }

	void setFilename(const std::string filename);
	std::string filename();

	Charset& charset();

	ivf::Texture* texture();
	ivf::Material* material();

	void load();
};

}