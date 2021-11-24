#ifndef _CRotateCursor_h_
#define _CRotateCursor_h_

#include <ivf/Shape.h>
#include <ivf/Transform.h>
#include <ivf/ExtrArrow.h>

#include <ivfmath/Vec3d.h>

#include "CircularTube.h"

IvfSmartPointer(VectorCursor);

class VectorCursor : public ivf::Shape {
public:
	enum TCursorType {
		CT_VECTOR,
		CT_ALPHA,
		CT_BETA
	};
private:
	ivf::TransformPtr	m_cursor;
	ivf::TransformPtr	m_alfaRotate;
	ivf::TransformPtr	m_betaRotate;
	ivf::TransformPtr	m_nonRotatingCursor;
	ivf::TransformPtr	m_angleIndicators;
	CircularTubePtr		m_circle1;
	CircularTubePtr		m_circle2;
	ivf::TransformPtr	m_spheres;
	ivf::ExtrArrowPtr	m_vectorIndicator;
	TCursorType			m_cursorType;
	double				m_alpha;
	double				m_beta;
	ivf::Vec3d			m_vector;

	ivf::TransformPtr	m_spheres1;
	ivf::TransformPtr	m_spheres2;

	void calcVector();
public:
	ivf::Vec3d& getDirection();
	void reset();
	double getBeta();
	double getAlpha();
	VectorCursor();
	virtual ~VectorCursor();

	IvfStdFactory(VectorCursor);

	void setType(TCursorType type);
	void setBeta(double beta);
	void setAlpha(double alpha);
protected:
	virtual void doCreateGeometry();
};

#endif
