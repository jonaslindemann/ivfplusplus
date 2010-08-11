#include <ivfglut/IvfGlutBase.h>

#include <ivf/IvfGL.h>

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif 

#include <ivfglut/IvfGlutApplication.h>

IvfSmartPointer(CIvfGlutBase);

CIvfGlutBase::CIvfGlutBase(int X, int Y, int W, int H)
:CIvfWidgetBase()
{
	m_id = -1;
	m_caption = "Glut Window";
	m_pos[0] = X;
	m_pos[1] = Y;
	m_size[0] = W;
	m_size[1] = H;
	CIvfGlutApplication* application = CIvfGlutApplication::getInstance();
	application->addWindow(this);
}

CIvfGlutBase::~CIvfGlutBase()
{
	
}

void CIvfGlutBase::create()
{
	m_id = glutCreateWindow(m_caption.c_str());
	glutPositionWindow(m_pos[0], m_pos[1]);
	glutReshapeWindow(m_size[0], m_size[1]);
}

void CIvfGlutBase::doRedraw()
{
	glutPostRedisplay();
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
		this->doMouseDown(x, y);
		break;
	case GLUT_UP:
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
	glutSetWindow(m_id);
	glutShowWindow();
}

void CIvfGlutBase::setId(int id)
{
	m_id = id;
}

int CIvfGlutBase::getId()
{
	return m_id;
}




