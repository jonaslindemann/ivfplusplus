#ifndef _CStarField_h_
#define _CStarField_h_

#include <ivf/IvfShape.h>
#include <ivf/IvfPointSet.h>
#include <ivf/IvfQuadSet.h>
#include <ivf/IvfSphere.h>
#include <ivf/IvfCamera.h>

IvfSmartPointer(CStarField);

class CStarField : public CIvfShape {
private:
	//CIvfPointSet* m_stars;
	CIvfQuadSetPtr m_texturedStars;
	CIvfQuadSetPtr m_planets;
	CIvfPointSetPtr m_pointStars;
	CIvfSpherePtr m_nebulaSphere;
	CIvfCameraPtr m_camera;

	void initializeStars();
	void getPolarVectors(
		double alfa,  double beta, 
		CIvfVec3d &normal, CIvfVec3d &s, CIvfVec3d &t);
public:
	CStarField(CIvfCamera* camera);
	virtual ~CStarField();

	void setCamera(CIvfCamera* camera);
protected:
	virtual void createGeometry();
};

#endif 
