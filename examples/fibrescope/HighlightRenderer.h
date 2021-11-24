#ifndef _CIvfHighlightRenderer_h_
#define _CIvfHighlightRenderer_h_

#include <ivf/Shape.h>

IvfSmartPointer(HighlightRenderer);

class HighlightRenderer: public ivf::Shape {
private:
	GLUquadricObj* m_qobj;
public:
	HighlightRenderer();
	virtual ~HighlightRenderer();

	IvfClassInfo("HighlightRenderer", ivf::Shape);
	IvfStdFactory(HighlightRenderer);
protected:
	virtual void doCreateGeometry();
};

#endif
