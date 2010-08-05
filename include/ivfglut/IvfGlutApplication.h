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

#include <ivfglut/IvfGlutWindow.h>

class CIvfGlutApplication {
private:
	static CIvfGlutApplication* m_instance;
	static CIvfSingletonDestroyer<CIvfGlutApplication> m_destroyer;
	static CIvfGlutWindow* m_windows[];
	static void cbDisplay0();
	static void cbReshape0(int width, int height);
	static void cbDisplay1();
	static void cbReshape1(int width, int height);
	static void cbDisplay2();
	static void cbReshape2(int width, int height);
	static void cbDisplay3();
	static void cbReshape3(int width, int height);
	static void cbDisplay4();
	static void cbReshape4(int width, int height);
	static void cbDisplay5();
	static void cbReshape5(int width, int height);
	static void cbDisplay6();
	static void cbReshape6(int width, int height);
	static void cbDisplay7();
	static void cbReshape7(int width, int height);
	static void cbDisplay8();
	static void cbReshape8(int width, int height);
	static void cbDisplay9();
	static void cbReshape9(int width, int height);
	unsigned int m_displayMode;
	unsigned int m_nextWindow;
public:	
	static CIvfGlutApplication* getInstance(int* argc, char** argv);
	static CIvfGlutApplication* getInstance();
	
	void setDisplayMode(unsigned int mode);
	unsigned int getDisplayMode();
	
	bool addWindow(CIvfGlutWindow* window);
	
	void mainLoop();
	void run();
protected:
	/** Protected constructor (do not use) */
	CIvfGlutApplication(int* argc, char** argv);
	~CIvfGlutApplication();
	friend class CIvfSingletonDestroyer<CIvfGlutApplication>;
};

