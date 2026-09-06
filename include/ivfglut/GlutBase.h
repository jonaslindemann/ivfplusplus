#pragma once

#include <string>

#ifdef WIN32
#include <Windows.h>
#endif

#include <ivfwidget/WidgetBase.h>
#include <ivf/rc.h>

namespace ivf {

IvfSmartPointer(GlutBase)

class GlutBase : public WidgetBase {
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
	bool		m_blinnPhongEnabled;
	float		m_globalAmbient[4];
public:
	GlutBase(int X, int Y, int W, int H, bool fullScreen = false);
	virtual ~GlutBase();
	
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

	void enableBlinnPhongShader(float ambR = 0.2f, float ambG = 0.2f, float ambB = 0.2f, float ambA = 1.0f);
	void disableBlinnPhongShader();
	bool isBlinnPhongShaderEnabled() const;

	/**
	 * Choose the pipeline the library renders with.
	 *
	 * RenderProfile::Legacy turns the shader path off entirely and gives back
	 * the fixed-function behaviour, which is what an application that has not
	 * been ported wants. RenderProfile::Mixed, the default, draws converted
	 * classes through the shader and lets the rest fall back. Setting it does
	 * not create or discard a shader, so the two can be compared at runtime.
	 */
	void setRenderProfile(RenderProfile profile);

	/** Returns the profile the library is rendering with. */
	RenderProfile renderProfile() const;

protected:
	virtual void doRender() override;
};

}