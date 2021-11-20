#ifndef _CIvfHighlightRenderer_h_
#define _CIvfHighlightRenderer_h_

#include <ivf/IvfShape.h>

IvfSmartPointer(CHighlightRenderer);

class CHighlightRenderer: public CShape {
private:
	GLUquadricObj* m_qobj;
public:
	CHighlightRenderer();
	virtual ~CHighlightRenderer();

	IvfClassInfo("CHighlightRenderer", CShape);
protected:
	virtual void doCreateGeometry();
};

#endif
