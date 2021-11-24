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

#include "UserSettings.h"

using namespace ivf;

UserSettings* UserSettings::m_instance = 0;
SingletonDestroyer<UserSettings> UserSettings::m_destroyer;

UserSettings* UserSettings::getInstance () 
{
    if (m_instance == 0)  
    {  
		m_instance = new UserSettings(); 
		m_destroyer.setSingleton(m_instance);
    }
    return m_instance; 
}

UserSettings::UserSettings()
{
	m_scaleFactor = 1.0;
	m_showUsage = false;
	m_connectionMaterial = nullptr;
	m_colorMap = nullptr;
	m_connectionSize = 1.0;
	m_enlargeUsage = false;
	m_textureScale = 1.0;
	m_fibreRadius = 0.5;
	m_fibreSides = 6;
	m_camera = nullptr;
	m_fibreRep = 0;
	m_blendFibres = false;
	m_breakageLimit = 0.7;
	m_fibreLighting = false;
}

UserSettings::~UserSettings()
{

}

void UserSettings::setScaleFactor(double factor)
{
	m_scaleFactor = factor;
}

double UserSettings::getScaleFactor()
{
	return m_scaleFactor;
}

void UserSettings::setShowUsage(bool flag)
{
	m_showUsage = flag;
}

bool UserSettings::getShowUsage()
{
	return m_showUsage;
}

void UserSettings::setConnectionMaterial(Material *material)
{
	m_connectionMaterial = material;
}

Material* UserSettings::getConnectionMaterial()
{
	return m_connectionMaterial;
}

void UserSettings::setColorMap(ColorMap *colorMap)
{
	m_colorMap = colorMap;
}

ColorMap* UserSettings::getColorMap()
{
	return m_colorMap;
}

void UserSettings::setConnectionSize(double size)
{
	m_connectionSize = size;
}

double UserSettings::getConnectionSize()
{
	return m_connectionSize;
}

void UserSettings::setEnlargeUsage(bool flag)
{
	m_enlargeUsage = flag;
}

bool UserSettings::getEnlargeUsage()
{
	return m_enlargeUsage;
}

void UserSettings::setTextureScale(float scale)
{
	m_textureScale = scale;
}

float UserSettings::getTextureScale()
{
	return m_textureScale;
}

void UserSettings::setFibreRadius(double radius)
{
	m_fibreRadius = radius;
}

double UserSettings::getFibreRadius()
{
	return m_fibreRadius;
}

void UserSettings::setExtrusionSides(int sides)
{
	m_fibreSides = sides;
}

int UserSettings::getExtrusionSides()
{
	return m_fibreSides;
}

void UserSettings::setCamera(Camera *camera)
{
	m_camera = camera;
}

Camera* UserSettings::getCamera()
{
	return m_camera;
}

void UserSettings::setRepresentation(int rep)
{
	m_fibreRep = rep;
}

int UserSettings::getRepresentation()
{
	return m_fibreRep;
}

void UserSettings::setBlendFibres(bool flag)
{
	m_blendFibres = flag;
}

bool UserSettings::getBlendFibres(void)
{
	return m_blendFibres;
}

void UserSettings::addHighlightPoint(double x, double y, double z, double radius)
{
	Vec3dPtr point = new Vec3d(x, y, z);
	m_highlightPoints.push_back(Vec3dPtr(point));
	m_highlightRadius.push_back(radius);
}

void UserSettings::getHighlightPoint(int idx, double &x, double &y, double &z, double &radius)
{
	if ((idx>=0) && (idx<m_highlightPoints.size()))
	{
		Vec3dPtr point;
		point = m_highlightPoints[idx];
		radius = m_highlightRadius[idx];
		point->getComponents(x, y, z);
	}
}

int UserSettings::getHighlightPointSize()
{
	return static_cast<int>(m_highlightPoints.size());
}

void UserSettings::clearHighlightPoints()
{
	m_highlightPoints.clear();
	m_highlightRadius.clear();
}

void UserSettings::setBreakageLimit(double limit)
{
	m_breakageLimit = limit;
}

double UserSettings::getBreakageLimit()
{
	return m_breakageLimit;
}

void UserSettings::setFibreLighting(bool flag)
{
	m_fibreLighting = flag;
}

bool UserSettings::getFibreLighting()
{
	return m_fibreLighting;
}
