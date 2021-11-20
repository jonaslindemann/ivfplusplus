#ifndef _CRotateCursor_h_
#define _CRotateCursor_h_

#include <ivf/IvfShape.h>
#include <ivf/IvfTransform.h>
#include <ivf/IvfExtrArrow.h>

#include <ivfmath/IvfVec3d.h>

#include "IvfCircularTube.h"

IvfSmartPointer(CVectorCursor);

class CVectorCursor : public CShape {
public:
	enum TCursorType {
		CT_VECTOR,
		CT_ALPHA,
		CT_BETA
	};
private:
	CTransformPtr	m_cursor;
	CTransformPtr	m_alfaRotate;
	CTransformPtr	m_betaRotate;
	CTransformPtr	m_nonRotatingCursor;
	CTransformPtr	m_angleIndicators;
	CCircularTubePtr	m_circle1;
	CCircularTubePtr	m_circle2;
	CTransformPtr	m_spheres;
	CExtrArrowPtr	m_vectorIndicator;
	TCursorType			m_cursorType;
	double				m_alpha;
	double				m_beta;
	CVec3d			m_vector;

	CTransformPtr m_spheres1;
	CTransformPtr m_spheres2;

	void calcVector();
public:
	CVec3d& getDirection();
	void reset();
	double getBeta();
	double getAlpha();
	CVectorCursor();
	virtual ~CVectorCursor();

	IvfClassInfo("CVectorCursor",CShape);

	void setType(TCursorType type);
	void setBeta(double beta);
	void setAlpha(double alpha);
protected:
	virtual void doCreateGeometry();
};

#endif
