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

CGlutBase::CGlutBase(int X, int Y, int W, int H, bool fullScreen)
:CWidgetBase()
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

	CGlutApplication* application = CGlutApplication::getInstance();
	application->addWindow(this);
}

CGlutBase::~CGlutBase()
{
	
}

void CGlutBase::create()
{
	if (m_fullScreen)
	{

	}
	else
	{
		m_id = glutCreateWindow(m_caption.c_str());
		glutPositionWindow(m_pos[0], m_pos[1]);
		glutReshapeWindow(m_size[0], m_size[1]);

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}
		fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	}
}

double CGlutBase::doElapsedTime()
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

void CGlutBase::doEnableTimeout(float time, int nbr)
{
	CGlutApplication::getInstance()->enableTimer(nbr, (int)(time*1000.0));
}

void CGlutBase::doDisableTimeout(int nbr)
{
	CGlutApplication::getInstance()->disableTimer(nbr);
}

void CGlutBase::glutDisplay()
{
	this->doDraw();
	glutSwapBuffers();
}

void CGlutBase::glutReshape(int width, int height)
{
	this->doResize(width, height);
}

void CGlutBase::glutKeyboard(unsigned char key, int x, int y)
{
	this->doKeyboard(key, x, y);
}

void CGlutBase::glutMouse(int button, int state, int x, int y)
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
			setModifierKey(CWidgetBase::MT_SHIFT);
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
			setModifierKey(CWidgetBase::MT_CTRL);
		if (glutGetModifiers() == GLUT_ACTIVE_ALT)
			setModifierKey(CWidgetBase::MT_ALT);
		this->doMouseDown(x, y);
		break;
	case GLUT_UP:
		setModifierKey(CWidgetBase::MT_NONE);
		this->doMouseUp(x, y);
		this->clearMouseStatus();
		break;
	}
}

void CGlutBase::glutMotion(int x, int y)
{
	this->doMouseMove(x, y);
}

void CGlutBase::glutPassiveMotion(int x, int y)
{
	this->doMouseMove(x, y);
}

void CGlutBase::glutTimer0(int value)
{
	if (this->doTimeout0())
		CGlutApplication::getInstance()->enableTimer(0, (int)(this->getTimeout(0)*1000.0));
	else
		CGlutApplication::getInstance()->disableTimer(0);
}

void CGlutBase::glutTimer1(int value)
{
	if (this->doTimeout1())
		CGlutApplication::getInstance()->enableTimer(1, (int)(this->getTimeout(1)*1000.0));
	else
		CGlutApplication::getInstance()->disableTimer(1);
}

void CGlutBase::glutTimer2(int value)
{
	if (this->doTimeout2())
		CGlutApplication::getInstance()->enableTimer(2, (int)(this->getTimeout(2)*1000.0));
	else
		CGlutApplication::getInstance()->disableTimer(2);
}

void CGlutBase::glutTimer3(int value)
{
	if (this->doTimeout3())
		CGlutApplication::getInstance()->enableTimer(3, (int)(this->getTimeout(3)*1000.0));
	else
		CGlutApplication::getInstance()->disableTimer(3);
}

void CGlutBase::glutTimer4(int value)
{
	if (this->doTimeout4())
		CGlutApplication::getInstance()->enableTimer(4, (int)(this->getTimeout(4)*1000.0));
	else
		CGlutApplication::getInstance()->disableTimer(4);
}

void CGlutBase::glutTimer5(int value)
{
	if (this->doTimeout5())
		CGlutApplication::getInstance()->enableTimer(5, (int)(this->getTimeout(5)*1000.0));
	else
		CGlutApplication::getInstance()->disableTimer(5);
}

void CGlutBase::glutTimer6(int value)
{
	if (this->doTimeout6())
		CGlutApplication::getInstance()->enableTimer(6, (int)(this->getTimeout(6)*1000.0));
	else
		CGlutApplication::getInstance()->disableTimer(6);
}

void CGlutBase::glutTimer7(int value)
{
	if (this->doTimeout7())
		CGlutApplication::getInstance()->enableTimer(7, (int)(this->getTimeout(7)*1000.0));
	else
		CGlutApplication::getInstance()->disableTimer(7);
}

void CGlutBase::glutTimer8(int value)
{
	if (this->doTimeout8())
		CGlutApplication::getInstance()->enableTimer(8, (int)(this->getTimeout(8)*1000.0));
	else
		CGlutApplication::getInstance()->disableTimer(8);
}

void CGlutBase::glutTimer9(int value)
{
	if (this->doTimeout9())
		CGlutApplication::getInstance()->enableTimer(9, (int)(this->getTimeout(9)*1000.0));
	else
		CGlutApplication::getInstance()->disableTimer(9);
}

void CGlutBase::setCaption(const std::string& caption)
{
	m_caption = caption;
	glutSetWindowTitle(const_cast<char*>(m_caption.c_str()));
}

const std::string CGlutBase::getCaption()
{
	return m_caption;
}

void CGlutBase::setWindowTitle(const std::string& title)
{
	this->setCaption(title);
}

const std::string CGlutBase::getWindowTitle()
{
	return this->getCaption();
}

void CGlutBase::show()
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

void CGlutBase::doFullscreen()
{
	glutSetWindow(m_id);
	glutFullScreen();
}

void CGlutBase::doRedraw()
{
	glutPostRedisplay();
}

void CGlutBase::setId(int id)
{
	m_id = id;
}

int CGlutBase::getId()
{
	return m_id;
}

void CGlutBase::setModeString(const std::string& modeString)
{
	m_modeString = modeString;
}

void CGlutBase::enterFullscreen()
{
	glutGameModeString(m_modeString.c_str());
	glutEnterGameMode();
}

