//
// FibreScope - Fibre network post processor
// Copyright (C) 2000-2003 Division of Structural Mecahnics, Lund University
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

#include "IvfFibre.h"

#include <ivf/ivfconfig.h>

//#include <GL/tube.h>
#include <GL/gle.h>

#include <ivfmath/IvfVec3d.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int g_j;
CIvfVectorCoordList* g_vCoordList;
CIvfVectorCoordList* g_vUndeformedList;
CIvfArrayCoordList* g_aCoordList;
CIvfArrayCoordList* g_aUndeformedList;
float g_r1, g_g1, g_b1;
float g_r2, g_g2, g_b2;
double g_u1[3];
double g_u2[3];
double g_p1[3];
double g_p2[3];
double g_u[3];
double g_v[3];
double g_n[3];
double g_n1[3];
double g_n2[3];
double g_uv[3];
double g_radius;
double g_scale;
double g_upvector[3];
int g_nSides;
double g_x, g_y, g_z;
double g_dx, g_dy, g_dz;
double g_summedSpine[1000][3];
bool g_firstTime = true;

// Fibre band method II

double g_p3[3];   // Point p3 
double g_u3[3];   // Displacement 3 
double g_ef[3];   // Camera forward vector
double g_efp[3];  // Forward vector in plane
double g_eo1[3];  // Fibre orientation vector
double g_eo2[3];  // Fibre orientation vector
double g_l1;      // Length of segment1
double g_l2;      // Length of segment2
double g_pn[3];   // Normal vector of plane
double g_efn;     // ef*n
bool g_haveNext;  // Have a next segment
float g_texCoord;
float g_texScale;

void normalizedcross(double* u, double* v, double* n)

{

  double l;



  /* compute the cross product (u x v for right-handed [ccw]) */

  n[0] = u[1] * v[2] - u[2] * v[1];

  n[1] = u[2] * v[0] - u[0] * v[2];

  n[2] = u[0] * v[1] - u[1] * v[0];



  /* normalize */

  l = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);

  n[0] /= l;

  n[1] /= l;

  n[2] /= l;

}

CIvfFibre::CIvfFibre()
{
	m_discreteTime = NULL;
	m_coords = new CIvfCoordHist();
	g_upvector[0] = 0.0;
	g_upvector[1] = 1.0;
	g_upvector[2] = 1.0;
	m_sectionCoords = NULL;
	m_sectionNormals = NULL;

	m_fibreRadius = -1.0;
	
	if (g_firstTime)
	{
		int i;
		for (i=0; i<20; i++)
		{
			g_summedSpine[i][0] = 0.0;
			g_summedSpine[i][1] = 0.0;
			g_summedSpine[i][2] = 0.0;
		}
	}
}

CIvfFibre::~CIvfFibre()
{
	delete [] m_sectionCoords;
	delete [] m_sectionNormals;
}

void CIvfFibre::setDiscreteTime(CIvfDiscreteTime *discreteTime)
{
	m_discreteTime = discreteTime;
	m_coords->setDiscreteTime(m_discreteTime);
}

CIvfCoordHist* CIvfFibre::getCoordHist()
{
	return m_coords;
}

void CIvfFibre::createGeometry()
{
	// Get settings

	CIvfUserSettingsPtr userSettings = CIvfUserSettings::getInstance();

	g_radius = userSettings->getFibreRadius();
	g_nSides = userSettings->getExtrusionSides();
	g_scale = userSettings->getScaleFactor();
	g_texScale = userSettings->getTextureScale();

	if (userSettings->getBlendFibres())
	{
		glBlendFunc(GL_ONE, GL_ONE);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
	}

	// To compare the methods the following code is 
	// executed for all methods but only used in the 
	// extrusion method

	switch (userSettings->getRepresentation()) {
	case FIBRE_BAND1:

		////////////////////////////////////////////////////////////////
		// Fibre band method I
		
		g_vCoordList = (CIvfVectorCoordList*)m_coords->getList();
		g_vUndeformedList = (CIvfVectorCoordList*)m_coords->getList(0);

		// Get camera up-vector
		
		userSettings->getCamera()->getUpVector(g_uv);
		
		glBegin(GL_TRIANGLES);
		for (g_j=0; g_j<g_vCoordList->getSize()-1; g_j++)
		{
			// Get undeformed coordinates

			g_vUndeformedList->getCoord(g_j, g_u1[0], g_u1[1], g_u1[2]);
			g_vUndeformedList->getCoord(g_j+1, g_u2[0], g_u2[1], g_u2[2]);

			// Get displacements
			
			g_vCoordList->getCoord(g_j, g_p1[0], g_p1[1], g_p1[2]);
			g_vCoordList->getCoord(g_j+1, g_p2[0], g_p2[1], g_p2[2]);

			// Calculate segment coordinates
			
			if (g_vUndeformedList!=g_vCoordList)
			{
				g_p1[0] = g_u1[0] + g_p1[0]*g_scale;
				g_p1[1] = g_u1[1] + g_p1[1]*g_scale;
				g_p1[2] = g_u1[2] + g_p1[2]*g_scale;
				
				g_p2[0] = g_u2[0] + g_p2[0]*g_scale;
				g_p2[1] = g_u2[1] + g_p2[1]*g_scale;
				g_p2[2] = g_u2[2] + g_p2[2]*g_scale;
			}
			else
			{
				g_p1[0] = g_u1[0];
				g_p1[1] = g_u1[1];
				g_p1[2] = g_u1[2];
				
				g_p2[0] = g_u2[0];
				g_p2[1] = g_u2[1];
				g_p2[2] = g_u2[2];
			}

			// Calculate face normal
			
			g_u[0] = g_p2[0] - g_p1[0];
			g_u[1] = g_p2[1] - g_p1[1];
			g_u[2] = g_p2[2] - g_p1[2];
			
			g_v[0] = g_uv[0];
			g_v[1] = g_uv[1];
			g_v[2] = g_uv[2];
			
			normalizedcross(g_u,g_v,g_n);

			// OpenGL drawing
			
			glNormal3dv(g_n);
			
			glTexCoord2f(1.0,0.0);
			glVertex3d(g_p1[0]+g_uv[0]*g_radius, g_p1[1]+g_uv[1]*g_radius, g_p1[2]+g_uv[2]*g_radius);
			
			glTexCoord2f(0.0,0.0);
			glVertex3d(g_p1[0]-g_uv[0]*g_radius, g_p1[1]-g_uv[1]*g_radius, g_p1[2]-g_uv[2]*g_radius);
			
			glTexCoord2f(1.0,1.0);
			glVertex3d(g_p2[0]+g_uv[0]*g_radius, g_p2[1]+g_uv[1]*g_radius, g_p2[2]+g_uv[2]*g_radius);
			
			glTexCoord2f(0.0,0.0);
			glVertex3d(g_p1[0]-g_uv[0]*g_radius, g_p1[1]-g_uv[1]*g_radius, g_p1[2]-g_uv[2]*g_radius);
			
			glTexCoord2f(0.0,1.0);
			glVertex3d(g_p2[0]-g_uv[0]*g_radius, g_p2[1]-g_uv[1]*g_radius, g_p2[2]-g_uv[2]*g_radius);
			
			glTexCoord2f(1.0,1.0);
			glVertex3d(g_p2[0]+g_uv[0]*g_radius, g_p2[1]+g_uv[1]*g_radius, g_p2[2]+g_uv[2]*g_radius);
		}
		glEnd();
		break;
	
	case FIBRE_BAND2:

		////////////////////////////////////////////////////////////////
		// Fibre band method II

		g_vCoordList = (CIvfVectorCoordList*)m_coords->getList();
		g_vUndeformedList = (CIvfVectorCoordList*)m_coords->getList(0);

		// Get camera forward vector
		
		userSettings->getCamera()->getForwardVector(g_ef[0],g_ef[1],g_ef[2]);
		
		glBegin(GL_TRIANGLES);
		for (g_j=0; g_j<g_vCoordList->getSize()-1; g_j++)
		{

			// Check if there are more segments after this

			if (g_j+2<g_vCoordList->getSize())
				g_haveNext = true;
			else
				g_haveNext = false;

			// Get undeformed coordinates

			g_vUndeformedList->getCoord(g_j, g_u1[0], g_u1[1], g_u1[2]);
			g_vUndeformedList->getCoord(g_j+1, g_u2[0], g_u2[1], g_u2[2]);
			if (g_haveNext)
				g_vUndeformedList->getCoord(g_j+2, g_u3[0], g_u3[1], g_u3[2]);

			// Get displacements
			
			g_vCoordList->getCoord(g_j, g_p1[0], g_p1[1], g_p1[2]);
			g_vCoordList->getCoord(g_j+1, g_p2[0], g_p2[1], g_p2[2]);
			if (g_haveNext)
				g_vCoordList->getCoord(g_j+2, g_p3[0], g_p3[1], g_p3[2]);

			// Calculate segment coordinates
			
			if (g_vUndeformedList!=g_vCoordList)
			{
				g_p1[0] = g_u1[0] + g_p1[0]*g_scale;
				g_p1[1] = g_u1[1] + g_p1[1]*g_scale;
				g_p1[2] = g_u1[2] + g_p1[2]*g_scale;
				
				g_p2[0] = g_u2[0] + g_p2[0]*g_scale;
				g_p2[1] = g_u2[1] + g_p2[1]*g_scale;
				g_p2[2] = g_u2[2] + g_p2[2]*g_scale;
				
				if (g_haveNext)
				{
					g_p3[0] = g_u3[0] + g_p3[0]*g_scale;
					g_p3[1] = g_u3[1] + g_p3[1]*g_scale;
					g_p3[2] = g_u3[2] + g_p3[2]*g_scale;
				}
			}
			else
			{
				g_p1[0] = g_u1[0];
				g_p1[1] = g_u1[1];
				g_p1[2] = g_u1[2];
				
				g_p2[0] = g_u2[0];
				g_p2[1] = g_u2[1];
				g_p2[2] = g_u2[2];

				if (g_haveNext)
				{
					g_p3[0] = g_u3[0];
					g_p3[1] = g_u3[1];
					g_p3[2] = g_u3[2];
				}
			}

			// Calculate length p1p2

			g_l1 = sqrt(pow(g_p2[0]-g_p1[0],2)+pow(g_p2[1]-g_p1[1],2)+pow(g_p2[2]-g_p1[2],2));
			g_texCoord = (float)g_l1*g_texScale/g_radius;

			// |n0    |ni   |nn
			// p0     pi    pn

			// Calculate normal vector of plane1

			if (g_j==0)
			{
				g_pn[0] = (g_p2[0] - g_p1[0]); // /g_l1;
				g_pn[1] = (g_p2[1] - g_p1[1]); // /g_l1;
				g_pn[2] = (g_p2[2] - g_p1[2]);// /g_l1;

				// Calculate segment orientation
				
				normalizedcross(g_ef,g_pn,g_eo1);
				normalizedcross(g_pn,g_eo1,g_n1);
			}
			else
			{
				g_eo1[0] = g_eo2[0];
				g_eo1[1] = g_eo2[1];
				g_eo1[2] = g_eo2[2];
				
				g_n1[0]  = g_n2[0];
				g_n1[1]  = g_n2[1];
				g_n1[2]  = g_n2[2];
			}

			// If we have more segments after this we calculate the plane
			// for the next point

			if (g_haveNext)
			{
				// Calculate normal vector of plane2. Plane2 is averaged with plane1
				
				g_pn[0] = g_pn[0] + g_p3[0] - g_p2[0]; // /g_l2)/2.0;
				g_pn[1] = g_pn[1] + g_p3[1] - g_p2[1]; // /g_l2)/2.0;
				g_pn[2] = g_pn[2] + g_p3[2] - g_p2[2]; // /g_l2)/2.0;

				// Calculate segment orientation

				normalizedcross(g_ef,g_pn,g_eo2);
				normalizedcross(g_pn,g_eo2,g_n2);
			}
			else
			{
				// if single or last segment the same orientation is 
				// used at both ends.

				g_eo2[0] = g_eo1[0];
				g_eo2[1] = g_eo1[1];
				g_eo2[2] = g_eo1[2];
				normalizedcross(g_ef,g_pn,g_eo2);
				normalizedcross(g_pn,g_eo2,g_n2);
			}

			// Calculate face normal
			

			/*
			g_u[0] = g_p2[0] - g_p1[0];
			g_u[1] = g_p2[1] - g_p1[1];
			g_u[2] = g_p2[2] - g_p1[2];
			
			g_v[0] = g_eo1[0];
			g_v[1] = g_eo1[1];
			g_v[2] = g_eo1[2];
			
			normalizedcross(g_u,g_v,g_n);
			*/

			// OpenGL Drawing 

			glNormal3dv(g_n1);

			glTexCoord2f(1.0,0.0);
			glVertex3d(g_p1[0]+g_eo1[0]*g_radius, g_p1[1]+g_eo1[1]*g_radius, g_p1[2]+g_eo1[2]*g_radius);
			
			glTexCoord2f(0.0,0.0);
			glVertex3d(g_p1[0]-g_eo1[0]*g_radius, g_p1[1]-g_eo1[1]*g_radius, g_p1[2]-g_eo1[2]*g_radius);
			
			glNormal3dv(g_n2);

			glTexCoord2f(1.0,g_texCoord);
			glVertex3d(g_p2[0]+g_eo2[0]*g_radius, g_p2[1]+g_eo2[1]*g_radius, g_p2[2]+g_eo2[2]*g_radius);
			
			glNormal3dv(g_n1);

			glTexCoord2f(0.0,0.0);
			glVertex3d(g_p1[0]-g_eo1[0]*g_radius, g_p1[1]-g_eo1[1]*g_radius, g_p1[2]-g_eo1[2]*g_radius);
			
			glNormal3dv(g_n2);

			glTexCoord2f(0.0,g_texCoord);
			glVertex3d(g_p2[0]-g_eo2[0]*g_radius, g_p2[1]-g_eo2[1]*g_radius, g_p2[2]-g_eo2[2]*g_radius);
			
			glTexCoord2f(1.0,g_texCoord);
			glVertex3d(g_p2[0]+g_eo2[0]*g_radius, g_p2[1]+g_eo2[1]*g_radius, g_p2[2]+g_eo2[2]*g_radius);
		}
		glEnd();

		break;

	case FIBRE_EXTRUSION:
		
		////////////////////////////////////////////////////////////////
		// Extrusion method
		
		g_aCoordList = (CIvfArrayCoordList*)m_coords->getList();
		g_aUndeformedList = (CIvfArrayCoordList*)m_coords->getList(0);

		for (g_j=0; g_j<g_aCoordList->getSize(); g_j++)
		{
			g_summedSpine[g_j][0] = 0.0;
			g_summedSpine[g_j][1] = 0.0;
			g_summedSpine[g_j][2] = 0.0;						
			if (g_j<g_aCoordList->getSize())
			{
				if (g_aCoordList!=g_aUndeformedList)
				{
					g_aCoordList->getCoord(g_j, g_dx, g_dy, g_dz);
					g_aUndeformedList->getCoord(g_j, g_x, g_y, g_z);
					g_summedSpine[g_j][0] = g_dx*g_scale+g_x;
					g_summedSpine[g_j][1] = g_dy*g_scale+g_y;
					g_summedSpine[g_j][2] = g_dz*g_scale+g_z;
				}
				else
				{
					g_aUndeformedList->getCoord(g_j, g_x, g_y, g_z);
					g_summedSpine[g_j][0] = g_x;
					g_summedSpine[g_j][1] = g_y;
					g_summedSpine[g_j][2] = g_z;			
				}
			}
		}

		glMatrixMode(GL_TEXTURE);

		glPushMatrix();
		glScalef(g_texScale, g_texScale, 0.0);

		glMatrixMode(GL_MODELVIEW);

		//glDisable(GL_TEXTURE_2D);
		gleExtrusion(
			g_nSides+1,
			m_sectionCoords,
			m_sectionNormals,
			g_upvector,
			g_aCoordList->getSize(),
			g_summedSpine,
			NULL
			);
		//glEnable(GL_TEXTURE_2D);

		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		break;
	}

	if (userSettings->getBlendFibres())
	{
		glBlendFunc(GL_ONE, GL_ZERO);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

}

void CIvfFibre::saveToStream(std::ostream &out)
{
	m_coords->saveToStream(out);
}

void CIvfFibre::readFromStream(std::istream &in)
{
	m_coords->readFromStream(in);
}

void CIvfFibre::initSection()
{
	double r;
	int nSides;
	double angle;
	double x, y;
	int i;

	CIvfUserSettingsPtr userSettings = CIvfUserSettings::getInstance();
	
	r = userSettings->getFibreRadius();
	nSides = userSettings->getExtrusionSides();

	if (m_sectionCoords!=NULL)
		delete [] m_sectionCoords;
	if (m_sectionNormals!=NULL)
		delete [] m_sectionNormals;
	
	m_sectionCoords = new gleDouble[nSides+1][2];
	m_sectionNormals = new gleDouble[nSides+1][2];
	
	for (i = 0; i<=nSides; i++)
	{
		angle = 2.0*M_PI*( ((double)i) / ((double)nSides) );
		x = r * cos(angle);
		y = r * sin(angle);
		m_sectionCoords[i][0] = x;
		m_sectionCoords[i][1] = y;
		m_sectionNormals[i][0] = x/r;
		m_sectionNormals[i][1] = y/r;
	}
}

void CIvfFibre::refresh()
{
	this->initSection();
}


void CIvfFibre::setFibreRadius(double radius)
{
	m_fibreRadius = radius;
}
