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
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif 

#include <ivfglut/IvfGlutBase.h>

#define GLUT_CB(id) \
	static void cbDisplay##id(); \
	static void cbReshape##id(int width, int height); \
	static void cbKeyboard##id(unsigned char key, int x, int y); \
	static void cbMouse##id(int button, int state, int x, int y); \
	static void cbMotion##id(int x, int y); \
	static void cbPassiveMotion##id(int x, int y); 

class CIvfGlutApplication {
private:
	static CIvfGlutApplication* m_instance;
	static CIvfSingletonDestroyer<CIvfGlutApplication> m_destroyer;
	static CIvfGlutBase* m_windows[];
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
	unsigned int m_nextWindow;
public:	
	static CIvfGlutApplication* getInstance(int* argc, char** argv);
	static CIvfGlutApplication* getInstance();
	
	void setDisplayMode(unsigned int mode);
	unsigned int getDisplayMode();
	
	bool addWindow(CIvfGlutBase* window);
	
	void mainLoop();
	void run();
protected:
	/** Protected constructor (do not use) */
	CIvfGlutApplication(int* argc, char** argv);
	~CIvfGlutApplication();
	friend class CIvfSingletonDestroyer<CIvfGlutApplication>;
};

