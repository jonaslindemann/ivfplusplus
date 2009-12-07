#ifndef _CIvfRotateCursor_h_
#define _CIvfRotateCursor_h_

#include <ivf/IvfShape.h>
#include <ivf/IvfTransform.h>
#include <ivf/IvfExtrArrow.h>

#include <ivfmath/IvfVec3d.h>

#include "IvfCircularTube.h"

IvfSmartPointer(CIvfVectorCursor);

class CIvfVectorCursor : public CIvfShape {
public:
	enum TCursorType {
		CT_VECTOR,
		CT_ALPHA,
		CT_BETA
	};
private:
	CIvfTransformPtr	m_cursor;
	CIvfTransformPtr	m_alfaRotate;
	CIvfTransformPtr	m_betaRotate;
	CIvfTransformPtr	m_nonRotatingCursor;
	CIvfTransformPtr	m_angleIndicators;
	CIvfCircularTubePtr	m_circle1;
	CIvfCircularTubePtr	m_circle2;
	CIvfTransformPtr	m_spheres;
	CIvfExtrArrowPtr	m_vectorIndicator;
	TCursorType			m_cursorType;
	double				m_alpha;
	double				m_beta;
	CIvfVec3d			m_vector;

	CIvfTransformPtr m_spheres1;
	CIvfTransformPtr m_spheres2;

	void calcVector();
public:
	CIvfVec3d& getDirection();
	void reset();
	double getBeta();
	double getAlpha();
	CIvfVectorCursor();
	virtual ~CIvfVectorCursor();

	IvfClassInfo("CIvfVectorCursor",CIvfShape);

	void setType(TCursorType type);
	void setBeta(double beta);
	void setAlpha(double alpha);
protected:
	virtual void doCreateGeometry();
};

#endif
