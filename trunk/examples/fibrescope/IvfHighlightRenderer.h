#ifndef _CIvfHighlightRenderer_h_
#define _CIvfHighlightRenderer_h_

#include <ivf/IvfShape.h>

IvfSmartPointer(CIvfHighlightRenderer);

class CIvfHighlightRenderer: public CIvfShape {
private:
	GLUquadricObj* m_qobj;
public:
	CIvfHighlightRenderer();
	virtual ~CIvfHighlightRenderer();

	IvfClassInfo("CIvfHighlightRenderer", CIvfShape);
protected:
	virtual void createGeometry();
};

#endif
