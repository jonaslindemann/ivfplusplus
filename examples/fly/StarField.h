#ifndef _CStarField_h_
#define _CStarField_h_

#include <ivf/IvfShape.h>
#include <ivf/IvfPointSet.h>
#include <ivf/IvfQuadSet.h>
#include <ivf/IvfSphere.h>
#include <ivf/IvfCamera.h>

IvfSmartPointer(CStarField);

class CStarField : public CShape {
private:
	//CPointSet* m_stars;
	CQuadSetPtr m_texturedStars;
	CQuadSetPtr m_planets;
	CPointSetPtr m_pointStars;
	CSpherePtr m_nebulaSphere;
	CCameraPtr m_camera;

	void initializeStars();
	void getPolarVectors(
		double alfa,  double beta,
		CVec3d &normal, CVec3d &s, CVec3d &t);
public:
	CStarField(CCamera* camera);
	virtual ~CStarField();

	void setCamera(CCamera* camera);
protected:
	virtual void doCreateGeometry();
};

#endif
