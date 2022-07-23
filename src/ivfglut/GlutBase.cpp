#include <ivfglut/GlutBase.h>

#include <ivf/GL.h>

#ifndef __APPLE__
#define FREEGLUT_STATIC
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif 

#ifndef WIN32
#include <sys/time.h>
#endif 

#include <ivfglut/GlutApplication.h>

#include <cmath>
#include <cstdio>

using namespace ivf;

GlutBase::GlutBase(int X, int Y, int W, int H, bool fullScreen)
:WidgetBase()
{
	m_fullScreen = fullScreen;
	m_id = -1;
	m_caption = "Glut Window";
	m_pos[0] = X;
	m_pos[1] = Y;
	m_size[0] = W;
	m_size[1] = H;

#ifdef WIN32
	LARGE_INTEGER count;
	QueryPerformanceFrequency(&m_countsPerSec);
	QueryPerformanceCounter(&count);
	m_zeroTime = (double)(count.QuadPart)/(double)(m_countsPerSec.QuadPart);
#else
	timeval tv;
	gettimeofday(&tv, 0);
	m_zeroTime = (double)tv.tv_sec + 1e-6*(double)tv.tv_usec;
#endif
	m_elapsedTime = 0.0;

	GlutApplication* application = GlutApplication::getInstance();
	application->addWindow(this);
}

GlutBase::~GlutBase()
{
	
}

void GlutBase::create()
{
	if (m_fullScreen)
	{

	}
	else
	{
		m_id = glutCreateWindow(m_caption.c_str());
		glutPositionWindow(m_pos[0], m_pos[1]);
		glutReshapeWindow(m_size[0], m_size[1]);
	}
}

double GlutBase::doElapsedTime()
{
#ifdef WIN32
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	double currentTime = (double)(count.QuadPart)/(double)(m_countsPerSec.QuadPart);
	return currentTime - m_zeroTime;
#else
	timeval tv;
	gettimeofday(&tv, 0);
	return (double)tv.tv_sec + 1e-6*(double)tv.tv_usec - m_zeroTime;
#endif
}

void GlutBase::doEnableTimeout(float time, int nbr)
{
	GlutApplication::getInstance()->enableTimer(nbr, (int)(time*1000.0));
}

void GlutBase::doDisableTimeout(int nbr)
{
	GlutApplication::getInstance()->disableTimer(nbr);
}

void GlutBase::glutDisplay()
{
	this->doDraw();
	glutSwapBuffers();
}

void GlutBase::glutReshape(int width, int height)
{
	this->doResize(width, height);
}

void GlutBase::glutKeyboard(unsigned char key, int x, int y)
{
	this->doKeyboard(key, x, y);
}

void GlutBase::glutMouse(int button, int state, int x, int y)
{
	this->clearMouseStatus();
	
	switch (button) {
	case GLUT_LEFT_BUTTON:
		this->setLeftButtonStatus(true);
		break;
	case GLUT_RIGHT_BUTTON:
		this->setRightButtonStatus(true);
		break;
	case GLUT_MIDDLE_BUTTON:
		this->setMiddleButtonStatus(true);
		break;
	}

	switch (state) {
	case GLUT_DOWN:
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
			setModifierKey(WidgetBase::MT_SHIFT);
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
			setModifierKey(WidgetBase::MT_CTRL);
		if (glutGetModifiers() == GLUT_ACTIVE_ALT)
			setModifierKey(WidgetBase::MT_ALT);
		this->doMouseDown(x, y);
		break;
	case GLUT_UP:
		setModifierKey(WidgetBase::MT_NONE);
		this->doMouseUp(x, y);
		this->clearMouseStatus();
		break;
	}
}

void GlutBase::glutMotion(int x, int y)
{
	this->doMouseMove(x, y);
}

void GlutBase::glutPassiveMotion(int x, int y)
{
	this->doMouseMove(x, y);
}

void GlutBase::glutTimer0(int value)
{
	if (this->doTimeout0())
		GlutApplication::getInstance()->enableTimer(0, (int)(this->getTimeout(0)*1000.0));
	else
		GlutApplication::getInstance()->disableTimer(0);
}

void GlutBase::glutTimer1(int value)
{
	if (this->doTimeout1())
		GlutApplication::getInstance()->enableTimer(1, (int)(this->getTimeout(1)*1000.0));
	else
		GlutApplication::getInstance()->disableTimer(1);
}

void GlutBase::glutTimer2(int value)
{
	if (this->doTimeout2())
		GlutApplication::getInstance()->enableTimer(2, (int)(this->getTimeout(2)*1000.0));
	else
		GlutApplication::getInstance()->disableTimer(2);
}

void GlutBase::glutTimer3(int value)
{
	if (this->doTimeout3())
		GlutApplication::getInstance()->enableTimer(3, (int)(this->getTimeout(3)*1000.0));
	else
		GlutApplication::getInstance()->disableTimer(3);
}

void GlutBase::glutTimer4(int value)
{
	if (this->doTimeout4())
		GlutApplication::getInstance()->enableTimer(4, (int)(this->getTimeout(4)*1000.0));
	else
		GlutApplication::getInstance()->disableTimer(4);
}

void GlutBase::glutTimer5(int value)
{
	if (this->doTimeout5())
		GlutApplication::getInstance()->enableTimer(5, (int)(this->getTimeout(5)*1000.0));
	else
		GlutApplication::getInstance()->disableTimer(5);
}

void GlutBase::glutTimer6(int value)
{
	if (this->doTimeout6())
		GlutApplication::getInstance()->enableTimer(6, (int)(this->getTimeout(6)*1000.0));
	else
		GlutApplication::getInstance()->disableTimer(6);
}

void GlutBase::glutTimer7(int value)
{
	if (this->doTimeout7())
		GlutApplication::getInstance()->enableTimer(7, (int)(this->getTimeout(7)*1000.0));
	else
		GlutApplication::getInstance()->disableTimer(7);
}

void GlutBase::glutTimer8(int value)
{
	if (this->doTimeout8())
		GlutApplication::getInstance()->enableTimer(8, (int)(this->getTimeout(8)*1000.0));
	else
		GlutApplication::getInstance()->disableTimer(8);
}

void GlutBase::glutTimer9(int value)
{
	if (this->doTimeout9())
		GlutApplication::getInstance()->enableTimer(9, (int)(this->getTimeout(9)*1000.0));
	else
		GlutApplication::getInstance()->disableTimer(9);
}

void GlutBase::setCaption(const std::string& caption)
{
	m_caption = caption;
	glutSetWindowTitle(const_cast<char*>(m_caption.c_str()));
}

const std::string GlutBase::getCaption()
{
	return m_caption;
}

void GlutBase::setWindowTitle(const std::string& title)
{
	this->setCaption(title);
}

const std::string GlutBase::getWindowTitle()
{
	return this->getCaption();
}

void GlutBase::show()
{
	if (this->m_fullScreen)
	{
		this->enterFullscreen();
	}
	else
	{
		glutSetWindow(m_id);
		glutShowWindow();
	}
}

void GlutBase::doFullscreen()
{
	glutSetWindow(m_id);
	glutFullScreen();
}

void GlutBase::doRedraw()
{
	glutPostRedisplay();
}

void GlutBase::setId(int id)
{
	m_id = id;
}

int GlutBase::getId()
{
	return m_id;
}

void GlutBase::setModeString(const std::string& modeString)
{
	m_modeString = modeString;
}

void GlutBase::enterFullscreen()
{
	glutGameModeString(m_modeString.c_str());
	glutEnterGameMode();
}

