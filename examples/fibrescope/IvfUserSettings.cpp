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

#include "IvfUserSettings.h"

CUserSettings* CUserSettings::m_instance = 0;
CSingletonDestroyer<CUserSettings> CUserSettings::m_destroyer;

CUserSettings* CUserSettings::getInstance () 
{
    if (m_instance == 0)  
    {  
		m_instance = new CUserSettings(); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

CUserSettings::CUserSettings()
{
	m_scaleFactor = 1.0;
	m_showUsage = false;
	m_connectionMaterial = NULL;
	m_colorMap = NULL;
	m_connectionSize = 1.0;
	m_enlargeUsage = false;
	m_textureScale = 1.0;
	m_fibreRadius = 0.5;
	m_fibreSides = 6;
	m_camera = NULL;
	m_fibreRep = 0;
	m_blendFibres = false;
	m_breakageLimit = 0.7;
	m_fibreLighting = false;
}

CUserSettings::~CUserSettings()
{

}

void CUserSettings::setScaleFactor(double factor)
{
	m_scaleFactor = factor;
}

double CUserSettings::getScaleFactor()
{
	return m_scaleFactor;
}

void CUserSettings::setShowUsage(bool flag)
{
	m_showUsage = flag;
}

bool CUserSettings::getShowUsage()
{
	return m_showUsage;
}

void CUserSettings::setConnectionMaterial(CMaterial *material)
{
	m_connectionMaterial = material;
}

CMaterial* CUserSettings::getConnectionMaterial()
{
	return m_connectionMaterial;
}

void CUserSettings::setColorMap(CColorMap *colorMap)
{
	m_colorMap = colorMap;
}

CColorMap* CUserSettings::getColorMap()
{
	return m_colorMap;
}

void CUserSettings::setConnectionSize(double size)
{
	m_connectionSize = size;
}

double CUserSettings::getConnectionSize()
{
	return m_connectionSize;
}

void CUserSettings::setEnlargeUsage(bool flag)
{
	m_enlargeUsage = flag;
}

bool CUserSettings::getEnlargeUsage()
{
	return m_enlargeUsage;
}

void CUserSettings::setTextureScale(float scale)
{
	m_textureScale = scale;
}

float CUserSettings::getTextureScale()
{
	return m_textureScale;
}

void CUserSettings::setFibreRadius(double radius)
{
	m_fibreRadius = radius;
}

double CUserSettings::getFibreRadius()
{
	return m_fibreRadius;
}

void CUserSettings::setExtrusionSides(int sides)
{
	m_fibreSides = sides;
}

int CUserSettings::getExtrusionSides()
{
	return m_fibreSides;
}

void CUserSettings::setCamera(CCamera *camera)
{
	m_camera = camera;
}

CCamera* CUserSettings::getCamera()
{
	return m_camera;
}

void CUserSettings::setRepresentation(int rep)
{
	m_fibreRep = rep;
}

int CUserSettings::getRepresentation()
{
	return m_fibreRep;
}

void CUserSettings::setBlendFibres(bool flag)
{
	m_blendFibres = flag;
}

bool CUserSettings::getBlendFibres(void)
{
	return m_blendFibres;
}

void CUserSettings::addHighlightPoint(double x, double y, double z, double radius)
{
	CVec3dPtr point = new CVec3d(x, y, z);
	m_highlightPoints.push_back(CVec3dPtr(point));
	m_highlightRadius.push_back(radius);
}

void CUserSettings::getHighlightPoint(int idx, double &x, double &y, double &z, double &radius)
{
	if ((idx>=0) && (idx<m_highlightPoints.size()))
	{
		CVec3dPtr point;
		point = m_highlightPoints[idx];
		radius = m_highlightRadius[idx];
		point->getComponents(x, y, z);
	}
}

int CUserSettings::getHighlightPointSize()
{
	return m_highlightPoints.size();
}

void CUserSettings::clearHighlightPoints()
{
	m_highlightPoints.clear();
	m_highlightRadius.clear();
}

void CUserSettings::setBreakageLimit(double limit)
{
	m_breakageLimit = limit;
}

double CUserSettings::getBreakageLimit()
{
	return m_breakageLimit;
}

void CUserSettings::setFibreLighting(bool flag)
{
	m_fibreLighting = flag;
}

bool CUserSettings::getFibreLighting()
{
	return m_fibreLighting;
}
