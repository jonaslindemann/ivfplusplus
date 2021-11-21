//
// Copyright 1999-2021 by Structural Mechanics, Lund University.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "jonas.lindemann@lunarc.lu.se".
//
//
// Written by Jonas Lindemann
//

#pragma once

#include <ivffile/FileReader.h>

#include <ivf/Composite.h>
#include <ivf/PolySet.h>
#include <ivf/MaterialSet.h>
#include <ivf/LineStripSet.h>
#include <ivf/Lighting.h>

#include <ivfmath/Vec3d.h>

namespace ivf {

#define OT_NONE   -1
#define OT_WORLD   0
#define OT_POLY    1
#define OT_GROUP   2  
#define OT_LIGHT   3

IvfSmartPointer(CAc3DReader);

/**
 * Base class for a AC3D reader
 *
 * This class implements a loader for AC3D files. The class
 * can handle surfaces with 3 or 4 vertices and textures.
 * It should load most AC3D files, but no guarantees are made.
 *
 * @author Jonas Lindemann
 */
class IVFFILE_API CAc3DReader : public CFileReader {
public:
	enum TSurfaceType {
		ST_POLY,
		ST_POLY_LINE
	};
private:
	void clearVertexList();
	std::string m_row;
	long m_currentObject;
	unsigned char m_currentSurfaceType;
	long m_currentNumberOfKids;
	std::string m_currentName;
	long m_currentMaterialIdx;
	long m_kidsLeft;

	TSurfaceType m_surfaceType;

	CCompositePtr m_world;
	CCompositePtr m_currentGroup;
	CMaterialPtr m_currentMaterial;
	CPolySetPtr m_currentPolySet;
	CLightPtr m_currentLight;
	CLineStripSetPtr m_currentLineStripSet;
	CIndexPtr m_currentIndex;
	CMaterialSetPtr m_materialSet;
	CShapePtr m_currentShape;
	CTexturePtr m_lastTexture;
	CLightingPtr m_lighting;

	std::vector<CVec3dPtr> m_vertexList;

	int			    m_lightCount;

	CCompositePtr  m_groupStack[100];
	int				m_childStack[100];
	int				m_currentLevel;

	bool			m_haveKids;
	bool			m_haveGeometry;

	double			m_currentPos[3];

	int				m_surfacesRead;

	int				m_currentTextureIndex;

	double			m_texrep[2];


	int m_nSurfaces;

	bool checkHeader(std::istream &in);
	bool readData(std::istream &in);

	int getOptionalKeyword(std::string &row);
	int getKeyword(std::string &row);

	bool parseObject(std::string &row);
	bool parseMaterial(std::string &row);
	bool parseKids(std::string &row);
	bool parseRefs(std::string &row, std::istream &in);
	bool parseMat(std::string &row);
	bool parseNumsurf(std::string &row);
	bool parseNumvert(std::string &row, std::istream &in);
	bool parseUrl(std::string &row);
	bool parseLoc(std::string &row);
	bool parseRot(std::string &row);
	bool parseTexture(std::string &row);
	bool parseData(std::string &row);
	bool parseName(std::string &row);
	bool parseSurf(std::string &row);
	bool parseTexrep(std::string &row);

	bool haveMoreChildren();
	void decChild();
	CComposite* currentGroup();
	void popGroup();
	void pushGroup(CComposite* group);
	void initGroupStack();
public:
	/** CAc3DReader constructor */
	CAc3DReader();

	/** CAc3DReader destructor */
	virtual ~CAc3DReader();

	IvfClassInfo("CAc3DReader",CFileReader);
	IvfStdFactory(CAc3DReader);

	/** Reads AC3D file */
	virtual void read();
};

/** \example ac3dreader.cpp */

}