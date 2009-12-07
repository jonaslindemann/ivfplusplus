#include "IvfHighlightRenderer.h"

#include "IvfUserSettings.h"

CIvfHighlightRenderer::CIvfHighlightRenderer()
{
	// Add construction code here

	m_qobj = gluNewQuadric();
	gluQuadricDrawStyle(m_qobj, GLU_FILL);
	gluQuadricTexture(m_qobj, GL_TRUE);
	gluQuadricOrientation(m_qobj, GLU_OUTSIDE);
	gluQuadricNormals(m_qobj, GLU_SMOOTH);
}

CIvfHighlightRenderer::~CIvfHighlightRenderer()
{
	gluDeleteQuadric(m_qobj);
}


void CIvfHighlightRenderer::doCreateGeometry()
{
	CIvfUserSettingsPtr userSettings = CIvfUserSettings::getInstance();
	CIvfMaterialPtr material = new CIvfMaterial();
	material->setDiffuseColor(0.1f, 0.1f, 0.1f, 1.0f);
	material->setAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);

	int i;
	double x, y, z, r;

	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_LIGHTING);

	for (i=0; i<userSettings->getHighlightPointSize(); i++)
	{
		userSettings->getHighlightPoint(i, x, y, z, r);

		glPushMatrix();
		glTranslated(x, y, z);
		glPushMatrix();
		glRotated(90, 1.0, 0.0, 0.0);

		material->render();

		glDisable(GL_FOG);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		gluSphere(m_qobj, r, 12, 8);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_FOG);
		glPopMatrix();
		glPopMatrix();
	}

	userSettings->clearHighlightPoints();
}
