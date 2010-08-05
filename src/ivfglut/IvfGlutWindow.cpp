#include <ivfglut/IvfGlutWindow.h>

#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif 

#include <ivfglut/IvfGlutApplication.h>

CIvfGlutWindow::CIvfGlutWindow(int X, int Y, int W, int H)
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

CIvfGlutWindow::~CIvfGlutWindow()
{
	
}

void CIvfGlutWindow::create()
{
	m_id = glutCreateWindow(m_caption.c_str());
	glutPositionWindow(m_pos[0], m_pos[1]);
	glutReshapeWindow(m_size[0], m_size[1]);
}

void CIvfGlutWindow::glutDisplay()
{
	this->doDraw();
}

void CIvfGlutWindow::glutReshape(int width, int height)
{
	this->doResize(width, height);
}

void CIvfGlutWindow::setCaption(const std::string& caption)
{
	m_caption = caption;
	glutSetWindowTitle(m_caption.c_str());
}

const std::string CIvfGlutWindow::getCaption()
{
	return m_caption;
}

void CIvfGlutWindow::setWindowTitle(const std::string& title)
{
	this->setCaption(title);
}

const std::string CIvfGlutWindow::getWindowTitle()
{
	return this->getCaption();
}

void CIvfGlutWindow::show()
{
	glutSetWindow(m_id);
	glutShowWindow();
}

void CIvfGlutWindow::setId(int id)
{
	m_id = id;
}

int CIvfGlutWindow::getId()
{
	return m_id;
}




