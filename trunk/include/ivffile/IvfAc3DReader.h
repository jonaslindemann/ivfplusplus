//
// Copyright 1999-2006 by Structural Mechanics, Lund University.
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
// Please report all bugs and problems to "ivf@byggmek.lth.se".
//
//
// Written by Jonas Lindemann
//

#ifndef _CIvfAc3DReader_h_
#define _CIvfAc3DReader_h_

#include <ivffile/IvfFileReader.h>

#include <ivf/IvfComposite.h>
#include <ivf/IvfPolySet.h>
#include <ivf/IvfMaterialSet.h>
#include <ivf/IvfLineStripSet.h>
#include <ivf/IvfLighting.h>

#include <ivfmath/IvfVec3d.h>

#define OT_NONE   -1
#define OT_WORLD   0
#define OT_POLY    1
#define OT_GROUP   2  
#define OT_LIGHT   3

IvfSmartPointer(CIvfAc3DReader);

/**
 * Base class for a AC3D reader
 *
 * This class implements a loader for AC3D files. The class
 * can handle surfaces with 3 or 4 vertices and textures.
 * It should load most AC3D files, but no guarantees are made.
 *
 * @author Jonas Lindemann
 */
class IVFFILE_API CIvfAc3DReader : public CIvfFileReader {
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

	CIvfCompositePtr m_world;
	CIvfCompositePtr m_currentGroup;
	CIvfMaterialPtr m_currentMaterial;
	CIvfPolySetPtr m_currentPolySet;
	CIvfLightPtr m_currentLight;
	CIvfLineStripSetPtr m_currentLineStripSet;
	CIvfIndexPtr m_currentIndex;
	CIvfMaterialSetPtr m_materialSet;
	CIvfShapePtr m_currentShape;
	CIvfTexturePtr m_lastTexture;
	CIvfLightingPtr m_lighting;

	std::vector<CIvfVec3dPtr> m_vertexList;

	int			    m_lightCount;

	CIvfCompositePtr  m_groupStack[100];
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
	CIvfComposite* currentGroup();
	void popGroup();
	void pushGroup(CIvfComposite* group);
	void initGroupStack();
public:
	/** CIvfAc3DReader constructor */
	CIvfAc3DReader();

	/** CIvfAc3DReader destructor */
	virtual ~CIvfAc3DReader();

	IvfClassInfo("CIvfAc3DReader",CIvfFileReader);

	/** Reads AC3D file */
	virtual void read();
};

/** \example ac3dreader.cpp */

#endif 
