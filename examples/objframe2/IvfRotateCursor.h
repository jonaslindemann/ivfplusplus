#ifndef _CIvfRotateCursor_h_
#define _CIvfRotateCursor_h_

#include <ivf/IvfShape.h>

#include <ivf/IvfSplineTube.h>

IvfSmartPointer(CIvfRotateCursor);

class CIvfRotateCursor : public CIvfShape {
private:
	CIvfSpline3dPtr m_spline;
	CIvfSplineTubePtr m_splineTube;
public:
	CIvfRotateCursor();
	virtual ~CIvfRotateCursor();

	IvfClassInfo("CIvfRotateCursor",CIvfShape);
protected:
	virtual void createGeometry();
};

#endif 
