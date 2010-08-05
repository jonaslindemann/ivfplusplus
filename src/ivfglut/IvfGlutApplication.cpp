#include <ivfglut/IvfGlutApplication.h>

#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif 

#define GLUTCB_DISPLAY(name,id) \
void CIvfGlutApplication::cb##name##id() \
{ \
	if (m_windows[id]!=0) \
		m_windows[id]->glut##name(); \
}

#define GLUTCB_RESHAPE(name,id) \
void CIvfGlutApplication::cb##name##id(int width, int height) \
{ \
	if (m_windows[id]!=0) \
		m_windows[id]->glut##name(width, height); \
}

#define GLUT_WIN_CB(id) \
glutDisplayFunc(cbDisplay##id); \
glutReshapeFunc(cbReshape##id);

GLUTCB_DISPLAY(Display,0)
GLUTCB_DISPLAY(Display,1)
GLUTCB_DISPLAY(Display,2)
GLUTCB_DISPLAY(Display,3)
GLUTCB_DISPLAY(Display,4)
GLUTCB_DISPLAY(Display,5)
GLUTCB_DISPLAY(Display,6)
GLUTCB_DISPLAY(Display,7)
GLUTCB_DISPLAY(Display,8)
GLUTCB_DISPLAY(Display,9)

GLUTCB_RESHAPE(Reshape,0)
GLUTCB_RESHAPE(Reshape,1)
GLUTCB_RESHAPE(Reshape,2)
GLUTCB_RESHAPE(Reshape,3)
GLUTCB_RESHAPE(Reshape,4)
GLUTCB_RESHAPE(Reshape,5)
GLUTCB_RESHAPE(Reshape,6)
GLUTCB_RESHAPE(Reshape,7)
GLUTCB_RESHAPE(Reshape,8)
GLUTCB_RESHAPE(Reshape,9)

CIvfGlutWindow* CIvfGlutApplication::m_windows[] = {0};
CIvfGlutApplication* CIvfGlutApplication::m_instance = 0;
CIvfSingletonDestroyer<CIvfGlutApplication> CIvfGlutApplication::m_destroyer;

CIvfGlutApplication* CIvfGlutApplication::getInstance(int* argc, char** argv) 
{
    if (m_instance == 0)  
    {  
		m_instance = new CIvfGlutApplication(argc, argv); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

CIvfGlutApplication* CIvfGlutApplication::getInstance() 
{
    if (m_instance != 0)  
    	return m_instance;
    else 
    	return 0;
}


CIvfGlutApplication::CIvfGlutApplication(int* argc, char** argv) 
{ 		
	glutInit(argc, argv);
	this->setDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	m_nextWindow = 0;
}

CIvfGlutApplication::~CIvfGlutApplication() 
{ 

}

void CIvfGlutApplication::setDisplayMode(unsigned int mode)
{
	m_displayMode = mode;
	glutInitDisplayMode(mode);
}

unsigned int CIvfGlutApplication::getDisplayMode()
{
	return m_displayMode;
}

void CIvfGlutApplication::mainLoop()
{
	glutMainLoop();
}

void CIvfGlutApplication::run()
{
	this->mainLoop();
}

bool CIvfGlutApplication::addWindow(CIvfGlutWindow* window)
{
	m_windows[m_nextWindow] = window;
	m_windows[m_nextWindow]->create();
	
	switch (m_nextWindow) {
	case (0):
		GLUT_WIN_CB(0);
		break;
	case (1):
		GLUT_WIN_CB(1);
		break;
	case (2):
		GLUT_WIN_CB(2);
		break;
	case (3):
		GLUT_WIN_CB(3);
		break;
	case (4):
		GLUT_WIN_CB(4);
		break;
	case (5):
		GLUT_WIN_CB(5);
		break;
	case (6):
		GLUT_WIN_CB(6);
		break;
	case (7):
		GLUT_WIN_CB(7);
		break;
	case (8):
		GLUT_WIN_CB(8);
		break;
	case (9):
		GLUT_WIN_CB(9);
		break;
	}
	
	m_nextWindow++;
	
	return true;
}	