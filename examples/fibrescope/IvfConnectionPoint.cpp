//
// FibreScope - Fibre network post processor
// Copyright (C) 2000-2005 Division of Structural Mecahnics, Lund University
//
// Written by Jonas Lindemann
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Comments and suggestions to jonas.lindemann@byggmek.lth.se
//

#include "IvfConnectionPoint.h"

#include <ivf/IvfLighting.h>
#include <ivf/IvfBlending.h>

GLUquadricObj* CIvfConnectionPoint::m_qobj = NULL;
CIvfMaterial* CIvfConnectionPoint::m_material = NULL;

CIvfConnectionPoint::CIvfConnectionPoint()
{
	m_discreteTime = NULL;
	m_coords = new CIvfCoordHist();
	m_useColor = true;
	m_camera = NULL;
	m_connectionRadius = 1.0;

	if (m_material==NULL)
	{
		m_material = new CIvfMaterial();
		m_material->addReference();
		m_material->setDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
		m_material->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	}

	if (m_qobj==NULL)
	{
		m_qobj = gluNewQuadric();
		gluQuadricDrawStyle(m_qobj, GLU_FILL);
		gluQuadricTexture(m_qobj, GL_TRUE);
		gluQuadricOrientation(m_qobj, GLU_OUTSIDE);
		gluQuadricNormals(m_qobj, GLU_SMOOTH);
	}
	//this->setMaterial(m_material);
}

CIvfConnectionPoint::~CIvfConnectionPoint()
{
	if (m_qobj!=NULL)
	{
		gluDeleteQuadric(m_qobj);
		m_qobj = NULL;
	}
}

void CIvfConnectionPoint::setDiscreteTime(CIvfDiscreteTime *discreteTime)
{
	m_discreteTime = discreteTime;
	m_coords->setDiscreteTime(m_discreteTime);
}

CIvfCoordHist* CIvfConnectionPoint::getCoordHist()
{
	return m_coords;
}

void CIvfConnectionPoint::doCreateGeometry()
{
	CIvfCoordListPtr coordList = m_coords->getList();
	CIvfCoordListPtr undeformedList = m_coords->getList(0);
	float r1, g1, b1;
	float r, g, b;
	double u1[3];
	double p1[3];
	double radius;
	double scale;

	CIvfBlendingPtr blending = CIvfBlending::getInstance();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	CIvfLightingPtr lighting = CIvfLighting::getInstance();
	lighting->enable();

	radius = m_connectionRadius;

	undeformedList->getCoord(0, u1[0], u1[1], u1[2]);

	coordList->getCoord(0, p1[0], p1[1], p1[2]);

	CIvfUserSettingsPtr userSettings = CIvfUserSettings::getInstance();
	scale = userSettings->getScaleFactor();

	if (undeformedList!=coordList)
	{
		p1[0] = u1[0] + p1[0]*scale;
		p1[1] = u1[1] + p1[1]*scale;
		p1[2] = u1[2] + p1[2]*scale;
	}
	else
	{
		p1[0] = u1[0];
		p1[1] = u1[1];
		p1[2] = u1[2];
	}

	if (userSettings->getShowUsage())
	{
		coordList->getColor(0, r1, g1, b1);
		if (userSettings->getColorMap()!=NULL)
			userSettings->getColorMap()->getColor((double)r1, r, g, b);
		else
			colorScale((double)r1, r, g, b);

		if (r1>=1.0)
		{
			m_material->setDiffuseColor((float)r, (float)g, (float)b, 0.0f);
			m_material->setAmbientColor((float)r*0.6, (float)g*0.6, (float)b*0.6, 0.0f);
		}
		else
		{
			m_material->setDiffuseColor((float)r, (float)g, (float)b, 1.0f);
			m_material->setAmbientColor((float)r*0.6, (float)g*0.6, (float)b*0.6, 1.0f);
		}

		m_material->render();
	}
	else
	{
		userSettings->getConnectionMaterial()->render();
	}

	glPushMatrix();
	glTranslated(p1[0],p1[1],p1[2]);

    double breakageLimit = userSettings->getBreakageLimit();

	if ((userSettings->getShowUsage())&&(userSettings->getEnlargeUsage()))
	{
		if ((r1>breakageLimit)&&(r1<1.0))
		{
			userSettings->addHighlightPoint(p1[0], p1[1], p1[2], m_connectionRadius*userSettings->getConnectionSize()*3.0);
			//blending->defineAddFunction();
			glPushMatrix();
			glRotated(90, 1.0, 0.0, 0.0);
			gluSphere(m_qobj, m_connectionRadius*userSettings->getConnectionSize(), 12, 8);

			/*
			m_material->setDiffuseColor(0.1f, 0.1f, 0.1f, 1.0f);
			m_material->setAmbientColor(0.4f, 0.4f, 0.4f, 1.0f);
			m_material->render();

			glDisable(GL_FOG);
			glDisable(GL_DEPTH_TEST);
			blending->enable();
			gluSphere(m_qobj, m_connectionRadius*userSettings->getConnectionSize()*3.0, 12, 8);
			blending->disable();
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_FOG);
			*/

			glPopMatrix();
		}
		else if (r1<=breakageLimit)
		{
			glPushMatrix();
			glRotated(90, 1.0, 0.0, 0.0);
			gluSphere(m_qobj, m_connectionRadius*userSettings->getConnectionSize()*1.0, 12, 8);
			glPopMatrix();
		}
	}
	else
	{
		if (r1<=1.0)
		{
			glPushMatrix();
			glRotated(90, 1.0, 0.0, 0.0);
			gluSphere(m_qobj, m_connectionRadius*userSettings->getConnectionSize(), 12, 8);
			glPopMatrix();
		}
	}

	//lighting->disable();

	glPopMatrix();
}

void CIvfConnectionPoint::setUseColor(bool flag)
{
	m_useColor = flag;
}

void CIvfConnectionPoint::saveToStream(ostream &out)
{
	m_coords->saveToStream(out);
}

void CIvfConnectionPoint::readFromStream(istream &in)
{
	m_coords->readFromStream(in);
}

void CIvfConnectionPoint::setCamera(CIvfCamera *camera)
{
	m_camera = camera;
}

void CIvfConnectionPoint::setRadius(double radius)
{
	m_connectionRadius = radius;
}


void CIvfConnectionPoint::colorScale(double t, float &r, float &g, float &b)
{
	if (t<0)
		t=0;
	else
	{
		if (t>1)
			t = 1.0;
	}

	r = 0.2 + 3*t;
	g = 0.2 + 3*t - 1;
	b = 0.2 + 3*t - 2;

	if (r>1.0)
		r = 1.0;

	if (g>1.0)
		g = 1.0;

	if (g<0.0)
		g = 0.0;

	if (b>1.0)
		b = 1.0;

	if (b<0.0)
		b = 0.0;

}

