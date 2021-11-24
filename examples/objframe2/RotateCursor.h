#ifndef _CIvfRotateCursor_h_
#define _CIvfRotateCursor_h_

#include <ivf/Shape.h>

#include <ivf/SplineTube.h>

IvfSmartPointer(RotateCursor);

class RotateCursor : public ivf::Shape {
private:
	ivf::Spline3dPtr m_spline;
	ivf::SplineTubePtr m_splineTube;
public:
	RotateCursor();
	virtual ~RotateCursor();

protected:
	virtual void doCreateGeometry();
};

#endif
