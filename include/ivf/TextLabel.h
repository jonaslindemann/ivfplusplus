#pragma once

#include <string>

#include <ivf/Billboard.h>
#include <ivf/Composite.h>
#include <ivf/QuadPlane.h>
#include <ivf/Texture.h>
#include <ivf/BitmapFont.h>

namespace ivf {

enum class HorizontalAlignment { Left, Center, Right };
enum class VerticalAlignment {Top, Middle, Bottom };

IvfSmartPointer(TextLabel);

class TextLabel : public ivf::BillBoard {
private:
	ivf::BitmapFontPtr m_font;
	std::string m_text;
	ivf::CompositePtr m_textQuads;
	float m_textWidth;
	float m_textHeight;
	float m_textSize;

	HorizontalAlignment m_alignX;
	VerticalAlignment m_alignY;
private:
	void updateText();
public:
	TextLabel();

	IvfClassInfo("TextLabel", Composite);

	static TextLabelPtr create() { return TextLabelPtr(new ivf::TextLabel()); }

	void setFont(ivf::BitmapFont* font);
	ivf::BitmapFont* font();

	void setText(const std::string text, float size=1.0);
	const std::string text();

	void setSize(float size);
	float size();

	void setAlignment(HorizontalAlignment alignX, VerticalAlignment alignY = ivf::VerticalAlignment::Bottom);

protected:
	virtual void doPreGeometry() override;
	virtual void doPostGeometry() override;
};

}