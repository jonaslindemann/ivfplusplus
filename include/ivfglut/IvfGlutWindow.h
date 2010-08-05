#pragma once

#include <string>

#include <ivfwidget/IvfWidgetBase.h>

IvfSmartPointer(CIvfGlutWindow)

class CIvfGlutWindow : public CIvfWidgetBase {
private:
	int m_id;
	std::string m_caption;
	int m_pos[2];
	int m_size[2];
public:
	CIvfGlutWindow(int X, int Y, int W, int H);
	virtual ~CIvfGlutWindow();
	
	void create();
	
	void show();
	
	void setCaption(const std::string& caption);
	const std::string getCaption();
	
	void setWindowTitle(const std::string& title);
	const std::string getWindowTitle();
	
	void setId(int id);
	int getId();
	
	void glutDisplay();
	void glutReshape(int width, int height);
};