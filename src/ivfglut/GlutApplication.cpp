#include <ivfglut/GlutApplication.h>

using namespace ivf;

#define GLUTCB_DISPLAY(id) \
void GlutApplication::cbDisplay##id() \
{ \
	if (m_windows[id]!=0) \
		m_windows[id]->glutDisplay(); \
}

#define GLUTCB_RESHAPE(id) \
void GlutApplication::cbReshape##id(int width, int height) \
{ \
	if (m_windows[id]!=0) \
		m_windows[id]->glutReshape(width, height); \
}

#define GLUTCB_KEYBOARD(id) \
void GlutApplication::cbKeyboard##id(unsigned char key, int x, int y) \
{ \
	if (m_windows[id]!=0) \
		m_windows[id]->glutKeyboard(key, x, y); \
}

#define GLUTCB_MOUSE(id) \
	void GlutApplication::cbMouse##id(int button, int state, int x, int y) \
	{ \
		if (m_windows[id]!=0) \
			m_windows[id]->glutMouse(button, state, x, y); \
	}

#define GLUTCB_MOTION(id) \
	void GlutApplication::cbMotion##id(int x, int y) \
	{ \
		if (m_windows[id]!=0) \
			m_windows[id]->glutMotion(x, y); \
	}

#define GLUTCB_PASSIVE_MOTION(id) \
	void GlutApplication::cbPassiveMotion##id(int x, int y) \
	{ \
		if (m_windows[id]!=0) \
			m_windows[id]->glutPassiveMotion(x, y); \
	}

#define GLUTCB_TIMER(id) \
	void GlutApplication::cbTimer##id(int value) \
	{ \
		if (value == 1) \
		{ \
			unsigned int i; \
			for (i=0; i<m_nextWindow; i++) \
			{ \
				if (m_windows[i]!=0) \
					m_windows[i]->glutTimer##id(value); \
			} \
		} \
	}


#define GLUT_WIN_CB(id) \
	glutDisplayFunc(cbDisplay##id); \
	glutReshapeFunc(cbReshape##id); \
	glutKeyboardFunc(cbKeyboard##id); \
	glutMouseFunc(cbMouse##id); \
	glutMotionFunc(cbMotion##id); \
	glutPassiveMotionFunc(cbPassiveMotion##id);

#define GLUT_TIMER_CB(id, msecs, value) \
	glutTimerFunc(msecs, cbTimer##id, value);
	
GLUTCB_DISPLAY(0)
GLUTCB_DISPLAY(1)
GLUTCB_DISPLAY(2)
GLUTCB_DISPLAY(3)
GLUTCB_DISPLAY(4)
GLUTCB_DISPLAY(5)
GLUTCB_DISPLAY(6)
GLUTCB_DISPLAY(7)
GLUTCB_DISPLAY(8)
GLUTCB_DISPLAY(9)

GLUTCB_RESHAPE(0)
GLUTCB_RESHAPE(1)
GLUTCB_RESHAPE(2)
GLUTCB_RESHAPE(3)
GLUTCB_RESHAPE(4)
GLUTCB_RESHAPE(5)
GLUTCB_RESHAPE(6)
GLUTCB_RESHAPE(7)
GLUTCB_RESHAPE(8)
GLUTCB_RESHAPE(9)

GLUTCB_KEYBOARD(0)
GLUTCB_KEYBOARD(1)
GLUTCB_KEYBOARD(2)
GLUTCB_KEYBOARD(3)
GLUTCB_KEYBOARD(4)
GLUTCB_KEYBOARD(5)
GLUTCB_KEYBOARD(6)
GLUTCB_KEYBOARD(7)
GLUTCB_KEYBOARD(8)
GLUTCB_KEYBOARD(9)

GLUTCB_MOUSE(0)
GLUTCB_MOUSE(1)
GLUTCB_MOUSE(2)
GLUTCB_MOUSE(3)
GLUTCB_MOUSE(4)
GLUTCB_MOUSE(5)
GLUTCB_MOUSE(6)
GLUTCB_MOUSE(7)
GLUTCB_MOUSE(8)
GLUTCB_MOUSE(9)

GLUTCB_MOTION(0)
GLUTCB_MOTION(1)
GLUTCB_MOTION(2)
GLUTCB_MOTION(3)
GLUTCB_MOTION(4)
GLUTCB_MOTION(5)
GLUTCB_MOTION(6)
GLUTCB_MOTION(7)
GLUTCB_MOTION(8)
GLUTCB_MOTION(9)

GLUTCB_PASSIVE_MOTION(0)
GLUTCB_PASSIVE_MOTION(1)
GLUTCB_PASSIVE_MOTION(2)
GLUTCB_PASSIVE_MOTION(3)
GLUTCB_PASSIVE_MOTION(4)
GLUTCB_PASSIVE_MOTION(5)
GLUTCB_PASSIVE_MOTION(6)
GLUTCB_PASSIVE_MOTION(7)
GLUTCB_PASSIVE_MOTION(8)
GLUTCB_PASSIVE_MOTION(9)

GLUTCB_TIMER(0)
GLUTCB_TIMER(1)
GLUTCB_TIMER(2)
GLUTCB_TIMER(3)
GLUTCB_TIMER(4)
GLUTCB_TIMER(5)
GLUTCB_TIMER(6)
GLUTCB_TIMER(7)
GLUTCB_TIMER(8)
GLUTCB_TIMER(9)

GlutBase* GlutApplication::m_windows[10] = {0};
GlutApplication* GlutApplication::m_instance = 0;
unsigned int GlutApplication::m_nextWindow = 0;
SingletonDestroyer<GlutApplication> GlutApplication::m_destroyer;

GlutApplication* GlutApplication::getInstance(int* argc, char** argv) 
{
    if (m_instance == 0)  
    {  
		m_instance = new GlutApplication(argc, argv); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

GlutApplication* GlutApplication::getInstance() 
{
    if (m_instance != 0)  
    	return m_instance;
    else 
    	return 0;
}


GlutApplication::GlutApplication(int* argc, char** argv) 
{ 	
#ifdef __APPLE__
    OSStatus err = noErr;
    err=GetCurrentProcess(&m_psn);
#endif 	
	glutInit(argc, argv);
	this->setDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	m_nextWindow = 0;
}

GlutApplication::~GlutApplication() 
{ 

}

void GlutApplication::setDisplayMode(unsigned int mode)
{
	m_displayMode = mode;
	glutInitDisplayMode(mode);
}

unsigned int GlutApplication::getDisplayMode()
{
	return m_displayMode;
}

void GlutApplication::mainLoop()
{
	glutMainLoop();
}

void GlutApplication::run()
{
	this->mainLoop();
}

void GlutApplication::runAppLoop(GlutBase *window)
{
#ifdef FREEGLUT
	bool finished = false;

	while (!finished)
	{
		glutMainLoop();
		finished = (!window->doAppLoop());
	}
#else
	bool finished = false;

	while (!finished)
	{
#ifdef __APPLE__
		CGEventRef mouseWheelEv=CGEventCreateScrollWheelEvent(nullptr,kCGScrollEventUnitPixel,1,(int32_t) 0);
		CGEventPostToPSN(&m_psn,mouseWheelEv);
		CFRelease(mouseWheelEv); 		
		glutCheckLoop();
		finished = (!window->doAppLoop());
#else
		glutMainLoop();		
#endif
	}
#endif
}

void GlutApplication::enableTimer(int timerIdx, int msecs)
{
	switch (timerIdx) {
	case (0):
		GLUT_TIMER_CB(0, msecs, 1);
		break;
	case (1):
		GLUT_TIMER_CB(1, msecs, 1);
		break;
	case (2):
		GLUT_TIMER_CB(2, msecs, 1);
		break;
	case (3):
		GLUT_TIMER_CB(3, msecs, 1);
		break;
	case (4):
		GLUT_TIMER_CB(4, msecs, 1);
		break;
	case (5):
		GLUT_TIMER_CB(5, msecs, 1);
		break;
	case (6):
		GLUT_TIMER_CB(6, msecs, 1);
		break;
	case (7):
		GLUT_TIMER_CB(7, msecs, 1);
		break;
	case (8):
		GLUT_TIMER_CB(8, msecs, 1);
		break;
	case (9):
		GLUT_TIMER_CB(9, msecs, 1);
		break;
	}
}

void GlutApplication::disableTimer(int timerIdx)
{
	switch (timerIdx) {
	case (0):
		GLUT_TIMER_CB(0, 10000, 0);
		break;
	case (1):
		GLUT_TIMER_CB(1, 10000, 0);
		break;
	case (2):
		GLUT_TIMER_CB(2, 10000, 0);
		break;
	case (3):
		GLUT_TIMER_CB(3, 10000, 0);
		break;
	case (4):
		GLUT_TIMER_CB(4, 10000, 0);
		break;
	case (5):
		GLUT_TIMER_CB(5, 10000, 0);
		break;
	case (6):
		GLUT_TIMER_CB(6, 10000, 0);
		break;
	case (7):
		GLUT_TIMER_CB(7, 10000, 0);
		break;
	case (8):
		GLUT_TIMER_CB(8, 10000, 0);
		break;
	case (9):
		GLUT_TIMER_CB(9, 10000, 0);
		break;
	}
}

bool GlutApplication::addWindow(GlutBase* window)
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
