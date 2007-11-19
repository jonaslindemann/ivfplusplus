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

CIvfUserSettings::CIvfUserSettings()
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
}

CIvfUserSettings::~CIvfUserSettings()
{

}

void CIvfUserSettings::setScaleFactor(double factor)
{
	m_scaleFactor = factor;
}

double CIvfUserSettings::getScaleFactor()
{
	return m_scaleFactor;
}

void CIvfUserSettings::setShowUsage(bool flag)
{
	m_showUsage = flag;
}

bool CIvfUserSettings::getShowUsage()
{
	return m_showUsage;
}

void CIvfUserSettings::setConnectionMaterial(CIvfMaterial *material)
{
	m_connectionMaterial = material;
}

CIvfMaterial* CIvfUserSettings::getConnectionMaterial()
{
	return m_connectionMaterial;
}

void CIvfUserSettings::setColorMap(CIvfColorMap *colorMap)
{
	m_colorMap = colorMap;
}

CIvfColorMap* CIvfUserSettings::getColorMap()
{
	return m_colorMap;
}

void CIvfUserSettings::setConnectionSize(double size)
{
	m_connectionSize = size;
}

double CIvfUserSettings::getConnectionSize()
{
	return m_connectionSize;
}

void CIvfUserSettings::setEnlargeUsage(bool flag)
{
	m_enlargeUsage = flag;
}

bool CIvfUserSettings::getEnlargeUsage()
{
	return m_enlargeUsage;
}

void CIvfUserSettings::setTextureScale(float scale)
{
	m_textureScale = scale;
}

float CIvfUserSettings::getTextureScale()
{
	return m_textureScale;
}

void CIvfUserSettings::setFibreRadius(double radius)
{
	m_fibreRadius = radius;
}

double CIvfUserSettings::getFibreRadius()
{
	return m_fibreRadius;
}

void CIvfUserSettings::setExtrusionSides(int sides)
{
	m_fibreSides = sides;
}

int CIvfUserSettings::getExtrusionSides()
{
	return m_fibreSides;
}

void CIvfUserSettings::setCamera(CIvfCamera *camera)
{
	m_camera = camera;
}

CIvfCamera* CIvfUserSettings::getCamera()
{
	return m_camera;
}

void CIvfUserSettings::setRepresentation(int rep)
{
	m_fibreRep = rep;
}

int CIvfUserSettings::getRepresentation()
{
	return m_fibreRep;
}
