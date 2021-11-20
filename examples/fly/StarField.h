#ifndef _CStarField_h_
#define _CStarField_h_

#include <ivf/Shape.h>
#include <ivf/PointSet.h>
#include <ivf/QuadSet.h>
#include <ivf/Sphere.h>
#include <ivf/Camera.h>

IvfSmartPointer(CStarField);

class CStarField : public ivf::CShape {
private:
	//CPointSet* m_stars;
	ivf::CQuadSetPtr m_texturedStars;
	ivf::CQuadSetPtr m_planets;
	ivf::CPointSetPtr m_pointStars;
	ivf::CSpherePtr m_nebulaSphere;
	ivf::CCameraPtr m_camera;

	void initializeStars();
	void getPolarVectors(
		double alfa,  double beta,
		ivf::CVec3d &normal, ivf::CVec3d &s, ivf::CVec3d &t);
public:
	CStarField(ivf::CCamera* camera);
	virtual ~CStarField();

	void setCamera(ivf::CCamera* camera);
protected:
	virtual void doCreateGeometry();
};

#endif
