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

enum class HorizontalAlignment { Left, Center, Right };
enum class VerticalAlignment {Top, Middle, Bottom };

IvfSmartPointer(BitmapFont);

class BitmapFont : public ivf::BillBoard {
private:
	Charset m_charset;
	std::string m_filename;
	std::string m_textureFilename;
	std::string m_text;
	ivf::TexturePtr m_fontTexture;
	ivf::MaterialPtr m_fontMaterial;
	ivf::CompositePtr m_textQuads;
	float m_textWidth;
	float m_textHeight;

	float m_textSize;

	HorizontalAlignment m_alignX;
	VerticalAlignment m_alignY;
private:
	void updateText();
public:
	BitmapFont(const std::string filename);

	IvfClassInfo("BitmapFont", Composite);

	static BitmapFontPtr create(const std::string filename) { return BitmapFontPtr(new BitmapFont(filename)); }

	void setFilename(const std::string filename);
	std::string filename();

	void setText(const std::string text, float size=1.0);
	const std::string text();

	void setSize(float size);
	float size();

	void setAlignment(HorizontalAlignment alignX, VerticalAlignment alignY = ivf::VerticalAlignment::Bottom);

	void load();

protected:
	virtual void doPreGeometry() override;
	virtual void doPostGeometry() override;
};

}