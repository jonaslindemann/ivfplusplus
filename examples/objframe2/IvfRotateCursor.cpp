#include "IvfRotateCursor.h"

CRotateCursor::CRotateCursor()
{
	m_spline = new CSpline3d();
	m_spline->setSize(3);
	m_spline->getPoint(0)->setComponents(1.0, 0.0, 0.0);
	m_spline->getPoint(1)->setComponents(cos(M_PI/4.0), 0.0, cos(M_PI/4.0));
	m_spline->getPoint(2)->setComponents(0.0, 0.0, 1.0);
	m_spline->setTangentWeight(0,0.0);
	m_spline->setTangentWeight(2,0.0);
	m_spline->update();

	m_splineTube = new CSplineTube();
	m_splineTube->setSpline(m_spline);
	m_splineTube->refresh();
}

CRotateCursor::~CRotateCursor()
{

}

void CRotateCursor::doCreateGeometry()
{
	m_splineTube->render();
}
