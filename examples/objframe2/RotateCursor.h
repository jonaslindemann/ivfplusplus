#ifndef _CIvfRotateCursor_h_
#define _CIvfRotateCursor_h_

#include <ivf/Shape.h>

#include <ivf/SplineTube.h>

IvfSmartPointer(CRotateCursor);

class CRotateCursor : public ivf::CShape {
private:
	ivf::CSpline3dPtr m_spline;
	ivf::CSplineTubePtr m_splineTube;
public:
	CRotateCursor();
	virtual ~CRotateCursor();

protected:
	virtual void doCreateGeometry();
};

#endif
