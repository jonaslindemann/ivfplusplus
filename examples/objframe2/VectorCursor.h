#ifndef _CRotateCursor_h_
#define _CRotateCursor_h_

#include <ivf/Shape.h>
#include <ivf/Transform.h>
#include <ivf/ExtrArrow.h>

#include <ivfmath/Vec3d.h>

#include "CircularTube.h"

IvfSmartPointer(CVectorCursor);

class CVectorCursor : public ivf::CShape {
public:
	enum TCursorType {
		CT_VECTOR,
		CT_ALPHA,
		CT_BETA
	};
private:
	ivf::CTransformPtr	m_cursor;
	ivf::CTransformPtr	m_alfaRotate;
	ivf::CTransformPtr	m_betaRotate;
	ivf::CTransformPtr	m_nonRotatingCursor;
	ivf::CTransformPtr	m_angleIndicators;
	CCircularTubePtr	m_circle1;
	CCircularTubePtr	m_circle2;
	ivf::CTransformPtr	m_spheres;
	ivf::CExtrArrowPtr	m_vectorIndicator;
	TCursorType			m_cursorType;
	double				m_alpha;
	double				m_beta;
	ivf::CVec3d			m_vector;

	ivf::CTransformPtr m_spheres1;
	ivf::CTransformPtr m_spheres2;

	void calcVector();
public:
	ivf::CVec3d& getDirection();
	void reset();
	double getBeta();
	double getAlpha();
	CVectorCursor();
	virtual ~CVectorCursor();

	void setType(TCursorType type);
	void setBeta(double beta);
	void setAlpha(double alpha);
protected:
	virtual void doCreateGeometry();
};

#endif
