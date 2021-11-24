#ifndef _CStarField_h_
#define _CStarField_h_

#include <ivf/Shape.h>
#include <ivf/PointSet.h>
#include <ivf/QuadSet.h>
#include <ivf/Sphere.h>
#include <ivf/Camera.h>

IvfSmartPointer(StarField);

class StarField : public ivf::Shape {
private:
	//PointSet* m_stars;
	ivf::QuadSetPtr m_texturedStars;
	ivf::QuadSetPtr m_planets;
	ivf::PointSetPtr m_pointStars;
	ivf::SpherePtr m_nebulaSphere;
	ivf::CameraPtr m_camera;

	void initializeStars();
	void getPolarVectors(
		double alfa,  double beta,
		ivf::Vec3d &normal, ivf::Vec3d &s, ivf::Vec3d &t);
public:
	StarField(ivf::Camera* camera);
	virtual ~StarField();

	void setCamera(ivf::Camera* camera);
protected:
	virtual void doCreateGeometry();
};

#endif
