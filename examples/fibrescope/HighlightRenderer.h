#ifndef _CIvfHighlightRenderer_h_
#define _CIvfHighlightRenderer_h_

#include <ivf/Shape.h>

IvfSmartPointer(CHighlightRenderer);

class CHighlightRenderer: public ivf::CShape {
private:
	GLUquadricObj* m_qobj;
public:
	CHighlightRenderer();
	virtual ~CHighlightRenderer();

	IvfClassInfo("CHighlightRenderer", ivf::CShape);
	IvfStdFactory(CHighlightRenderer);
protected:
	virtual void doCreateGeometry();
};

#endif
