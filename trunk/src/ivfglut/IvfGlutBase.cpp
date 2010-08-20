#include <ivfglut/IvfGlutBase.h>

#include <ivf/IvfGL.h>

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif 

#ifndef WIN32
#include <sys/time.h>
#endif 

#include <ivfglut/IvfGlutApplication.h>

IvfSmartPointer(CIvfGlutBase);

CIvfGlutBase::CIvfGlutBase(int X, int Y, int W, int H, bool fullScreen)
:CIvfWidgetBase()
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

	CIvfGlutApplication* application = CIvfGlutApplication::getInstance();
	application->addWindow(this);
}

CIvfGlutBase::~CIvfGlutBase()
{
	
}

void CIvfGlutBase::create()
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

double CIvfGlutBase::doElapsedTime()
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

void CIvfGlutBase::doEnableTimeout(float time, int nbr)
{
	CIvfGlutApplication::getInstance()->enableTimer(nbr, (int)time*1000.0);
}

void CIvfGlutBase::doDisableTimeout(int nbr)
{
	CIvfGlutApplication::getInstance()->disableTimer(nbr);
}

void CIvfGlutBase::glutDisplay()
{
	this->doDraw();
	glutSwapBuffers();
}

void CIvfGlutBase::glutReshape(int width, int height)
{
	this->doResize(width, height);
}

void CIvfGlutBase::glutKeyboard(unsigned char key, int x, int y)
{
	this->doKeyboard(key, x, y);
}

void CIvfGlutBase::glutMouse(int button, int state, int x, int y)
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
			setModifierKey(CIvfWidgetBase::MT_SHIFT);
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
			setModifierKey(CIvfWidgetBase::MT_CTRL);
		if (glutGetModifiers() == GLUT_ACTIVE_ALT)
			setModifierKey(CIvfWidgetBase::MT_ALT);
		this->doMouseDown(x, y);
		break;
	case GLUT_UP:
		setModifierKey(CIvfWidgetBase::MT_NONE);
		this->doMouseUp(x, y);
		this->clearMouseStatus();
		break;
	}
}

void CIvfGlutBase::glutMotion(int x, int y)
{
	this->doMouseMove(x, y);
}

void CIvfGlutBase::glutPassiveMotion(int x, int y)
{
	this->doMouseMove(x, y);
}

void CIvfGlutBase::glutTimer0(int value)
{
	if (this->doTimeout0())
		CIvfGlutApplication::getInstance()->enableTimer(0, (int)this->getTimeout(0)*1000.0);
	else
		CIvfGlutApplication::getInstance()->disableTimer(0);
}

void CIvfGlutBase::glutTimer1(int value)
{
	if (this->doTimeout1())
		CIvfGlutApplication::getInstance()->enableTimer(1, (int)this->getTimeout(1)*1000.0);
	else
		CIvfGlutApplication::getInstance()->disableTimer(1);
}

void CIvfGlutBase::glutTimer2(int value)
{
	if (this->doTimeout2())
		CIvfGlutApplication::getInstance()->enableTimer(2, (int)this->getTimeout(2)*1000.0);
	else
		CIvfGlutApplication::getInstance()->disableTimer(2);
}

void CIvfGlutBase::glutTimer3(int value)
{
	if (this->doTimeout3())
		CIvfGlutApplication::getInstance()->enableTimer(3, (int)this->getTimeout(3)*1000.0);
	else
		CIvfGlutApplication::getInstance()->disableTimer(3);
}

void CIvfGlutBase::glutTimer4(int value)
{
	if (this->doTimeout4())
		CIvfGlutApplication::getInstance()->enableTimer(4, (int)this->getTimeout(4)*1000.0);
	else
		CIvfGlutApplication::getInstance()->disableTimer(4);
}

void CIvfGlutBase::glutTimer5(int value)
{
	if (this->doTimeout5())
		CIvfGlutApplication::getInstance()->enableTimer(5, (int)this->getTimeout(5)*1000.0);
	else
		CIvfGlutApplication::getInstance()->disableTimer(5);
}

void CIvfGlutBase::glutTimer6(int value)
{
	if (this->doTimeout6())
		CIvfGlutApplication::getInstance()->enableTimer(6, (int)this->getTimeout(6)*1000.0);
	else
		CIvfGlutApplication::getInstance()->disableTimer(6);
}

void CIvfGlutBase::glutTimer7(int value)
{
	if (this->doTimeout7())
		CIvfGlutApplication::getInstance()->enableTimer(7, (int)this->getTimeout(7)*1000.0);
	else
		CIvfGlutApplication::getInstance()->disableTimer(7);
}

void CIvfGlutBase::glutTimer8(int value)
{
	if (this->doTimeout8())
		CIvfGlutApplication::getInstance()->enableTimer(8, (int)this->getTimeout(8)*1000.0);
	else
		CIvfGlutApplication::getInstance()->disableTimer(8);
}

void CIvfGlutBase::glutTimer9(int value)
{
	if (this->doTimeout9())
		CIvfGlutApplication::getInstance()->enableTimer(9, (int)this->getTimeout(9)*1000.0);
	else
		CIvfGlutApplication::getInstance()->disableTimer(9);
}

void CIvfGlutBase::setCaption(const std::string& caption)
{
	m_caption = caption;
	glutSetWindowTitle(const_cast<char*>(m_caption.c_str()));
}

const std::string CIvfGlutBase::getCaption()
{
	return m_caption;
}

void CIvfGlutBase::setWindowTitle(const std::string& title)
{
	this->setCaption(title);
}

const std::string CIvfGlutBase::getWindowTitle()
{
	return this->getCaption();
}

void CIvfGlutBase::show()
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

void CIvfGlutBase::doFullscreen()
{
	glutSetWindow(m_id);
	glutFullScreen();
}

void CIvfGlutBase::doRedraw()
{
	glutPostRedisplay();
}

void CIvfGlutBase::setId(int id)
{
	m_id = id;
}

int CIvfGlutBase::getId()
{
	return m_id;
}

void CIvfGlutBase::setModeString(const std::string& modeString)
{
	m_modeString = modeString;
}

void CIvfGlutBase::enterFullscreen()
{
	glutGameModeString(m_modeString.c_str());
	glutEnterGameMode();
}

