#ifndef _CIvfRotateCursor_h_
#define _CIvfRotateCursor_h_

#include <ivf/IvfShape.h>

#include <ivf/IvfSplineTube.h>

IvfSmartPointer(CRotateCursor);

class CRotateCursor : public CShape {
private:
	CSpline3dPtr m_spline;
	CSplineTubePtr m_splineTube;
public:
	CRotateCursor();
	virtual ~CRotateCursor();

	IvfClassInfo("CRotateCursor",CShape);
protected:
	virtual void doCreateGeometry();
};

#endif
