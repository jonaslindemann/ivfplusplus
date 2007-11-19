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

#ifndef _CIvfUserSettings_h_
#define _CIvfUserSettings_h_

#include <ivf/IvfBase.h>
#include <ivf/IvfMaterial.h>
#include "IvfColorMap.h"
#include <ivf/IvfCamera.h>

#define FIBRE_BAND1 0
#define FIBRE_BAND2 1
#define FIBRE_EXTRUSION 2

IvfSmartPointer(CIvfUserSettings);

class CIvfUserSettings : public CIvfBase {
private:
	CIvfMaterialPtr m_connectionMaterial;
	CIvfColorMapPtr m_colorMap;
	CIvfCameraPtr m_camera;

	double m_scaleFactor;
	bool m_showUsage;
	double m_connectionSize;
	bool m_enlargeUsage;
	float m_textureScale;
	double m_fibreRadius;
	int m_fibreSides;
	int m_fibreRep;
public:
	CIvfUserSettings();
	virtual ~CIvfUserSettings();

	void setScaleFactor(double factor);
	double getScaleFactor();

	void setShowUsage(bool flag);
	bool getShowUsage();

	void setFibreRadius(double radius);
	double getFibreRadius();

	void setTextureScale(float scale);
	float getTextureScale();

	void setEnlargeUsage(bool flag);
	bool getEnlargeUsage();

	double getConnectionSize();
	void setConnectionSize(double size);

	void setColorMap(CIvfColorMap* colorMap);
	CIvfColorMap* getColorMap();

	void setConnectionMaterial(CIvfMaterial* material);
	CIvfMaterial* getConnectionMaterial();

	void setCamera(CIvfCamera* camera);
	CIvfCamera* getCamera();

	void setExtrusionSides(int sides);
	int getExtrusionSides();

	void setRepresentation(int rep);
	int getRepresentation();
};

#endif 
