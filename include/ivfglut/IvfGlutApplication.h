#pragma once

//
// The following code based on code from the article
//
// To Kill a Singleton
// John Vlissides
//
// John Vlissides is a member of the research staff at IBM's
// Thomas J. Watson Research Center in Hawthorne, New York.
// He can be reached at vlis@watson.ibm.com
//

#include <ivfdef/IvfSingletonDestroyer.h>

#ifndef __APPLE__
#define FREEGLUT_STATIC
#include <GL/glut.h>
#ifdef FREEGLUT
#include <GL/freeglut_ext.h>
#endif
#else
#include <ApplicationServices/ApplicationServices.h>
#include <GLUT/glut.h>
#endif 

#define  IVF_RGB                           0x0000
#define  IVF_RGBA                          0x0000
#define  IVF_INDEX                         0x0001
#define  IVF_SINGLE                        0x0000
#define  IVF_DOUBLE                        0x0002
#define  IVF_ACCUM                         0x0004
#define  IVF_ALPHA                         0x0008
#define  IVF_DEPTH                         0x0010
#define  IVF_STENCIL                       0x0020
#define  IVF_MULTISAMPLE                   0x0080
#define  IVF_STEREO                        0x0100
#define  IVF_LUMINANCE                     0x0200

#include <ivfglut/IvfGlutBase.h>

#define GLUT_CB(id) \
	static void cbDisplay##id(); \
	static void cbReshape##id(int width, int height); \
	static void cbKeyboard##id(unsigned char key, int x, int y); \
	static void cbMouse##id(int button, int state, int x, int y); \
	static void cbMotion##id(int x, int y); \
	static void cbPassiveMotion##id(int x, int y); \
	static void cbTimer##id(int value);
	

class CIvfGlutApplication {
private:
	static CIvfGlutApplication* m_instance;
	static CIvfSingletonDestroyer<CIvfGlutApplication> m_destroyer;
	static CIvfGlutBase* m_windows[];
	static unsigned int m_nextWindow;
	GLUT_CB(0);
	GLUT_CB(1);
	GLUT_CB(2);
	GLUT_CB(3);
	GLUT_CB(4);
	GLUT_CB(5);
	GLUT_CB(6);
	GLUT_CB(7);
	GLUT_CB(8);
	GLUT_CB(9);
	unsigned int m_displayMode;
#ifdef __APPLE__
    ProcessSerialNumber m_psn;
#endif
	
public:	
	static CIvfGlutApplication* getInstance(int* argc, char** argv);
	static CIvfGlutApplication* getInstance();
	
	void setDisplayMode(unsigned int mode);
	unsigned int getDisplayMode();
	
	bool addWindow(CIvfGlutBase* window);
	void enableTimer(int timerIdx, int msecs);
	void disableTimer(int timerIdx);
	
	void mainLoop();
	void run();
	void runAppLoop(CIvfGlutBase *window);

protected:
	/** Protected constructor (do not use) */
	CIvfGlutApplication(int* argc, char** argv);
	~CIvfGlutApplication();
	friend class CIvfSingletonDestroyer<CIvfGlutApplication>;
};

