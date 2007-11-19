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
#include <ivf/IvfCamera.h>

#include <ivfdef/IvfSingletonDestroyer.h>

#include "IvfColorMap.h"

#define FIBRE_BAND1 0
#define FIBRE_BAND2 1
#define FIBRE_EXTRUSION 2

IvfStdPointer(CIvfUserSettings);

class CIvfUserSettings : public CIvfBase {
private:
	static CIvfUserSettings* m_instance;
	static CIvfSingletonDestroyer<CIvfUserSettings> m_destroyer;

	CIvfMaterialPtr m_connectionMaterial;
	CIvfColorMapPtr m_colorMap;
	CIvfCameraPtr m_camera;

	std::vector<CIvfVec3dPtr> m_highlightPoints;
	std::vector<double> m_highlightRadius;

	double m_scaleFactor;
	bool m_showUsage;
	double m_connectionSize;
	bool m_enlargeUsage;
	float m_textureScale;
	double m_fibreRadius;
	int m_fibreSides;
	int m_fibreRep;

	bool m_blendFibres;
	bool m_fibreLighting;

	double position[3];

	double m_breakageLimit;
public:
	static CIvfUserSettings* getInstance();

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

	void setBlendFibres(bool flag);
	bool getBlendFibres(void);

	void setBreakageLimit(double limit);
	double getBreakageLimit();

	void addHighlightPoint(double x, double y, double z, double radius);
	void getHighlightPoint(int idx, double &x, double &y, double &z, double &radius);
	int getHighlightPointSize();
	void clearHighlightPoints();

	void setFibreLighting(bool flag);
	bool getFibreLighting();

protected:
	/** Protected constructor (do not use) */
	CIvfUserSettings();
	~CIvfUserSettings();
	friend class CIvfSingletonDestroyer<CIvfUserSettings>;
};

#endif 
