#pragma once

#include <string>

#include <ivfwidget/IvfWidgetBase.h>

#ifdef WIN32
#include <windows.h>
#endif


IvfSmartPointer(CGlutBase)

class CGlutBase : public CWidgetBase {
private:
	int m_id;
	std::string m_caption;
	std::string m_modeString;
	int m_pos[2];
	int m_size[2];
#ifdef WIN32
	LARGE_INTEGER m_countsPerSec;
#endif
	double		m_elapsedTime;
	double		m_zeroTime;
	bool		m_fullScreen;
public:
	CGlutBase(int X, int Y, int W, int H, bool fullScreen = false);
	virtual ~CGlutBase();
	
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
	void glutKeyboard(unsigned char key, int x, int y);
	void glutMouse(int button, int state, int x, int y);
	void glutMotion(int x, int y);
	void glutPassiveMotion(int x, int y);
	void glutTimer0(int value);
	void glutTimer1(int value);
	void glutTimer2(int value);
	void glutTimer3(int value);
	void glutTimer4(int value);
	void glutTimer5(int value);
	void glutTimer6(int value);
	void glutTimer7(int value);
	void glutTimer8(int value);
	void glutTimer9(int value);

	virtual void doRedraw();

	double doElapsedTime();
	void doEnableTimeout(float time, int nbr);
	void doDisableTimeout(int nbr);
	void doFullscreen();

	void setModeString(const std::string& modeString);
	void enterFullscreen();
};