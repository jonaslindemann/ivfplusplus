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

#include <ivffile/IvfAc3DReader.h>

#include <ivfimage/IvfSgiImage.h>

#include <ivf/IvfTexture.h>

#include <sstream>

#define KW_ERROR       -1
#define KW_MATERIAL     0
#define KW_OBJECT       1
#define KW_SURF         2
#define KW_EMPTY_ROW	3

#define OKW_ERROR      -1
#define OKW_NAME		0
#define OKW_DATA		1
#define OKW_TEXTURE		2
#define OKW_TEXREP		3
#define OKW_ROT			4
#define OKW_LOC			5
#define OKW_URL			6
#define OKW_NUMVERT		7
#define OKW_NUMSURF		8
#define OKW_KIDS		9
#define OKW_MAT			10
#define OKW_REFS		11

using namespace std;

CIvfAc3DReader::CIvfAc3DReader()
{
	m_currentPolySet = NULL;
	m_materialSet = new CIvfMaterialSet();
	//m_materialSet->addReference();

	m_lighting = CIvfLighting::getInstance();
	m_lightCount = 0;
}

CIvfAc3DReader::~CIvfAc3DReader()
{
	//m_materialSet->deleteReference();
}

void CIvfAc3DReader::read()
{
	// Initiate max min calc

	resetSize();

	// Initialize variables

	initGroupStack();
	m_surfacesRead = 0;
	m_currentName = "noname";

	m_texrep[0] = 1.0;
	m_texrep[1] = 1.0;

	m_currentLight = NULL;

	m_lastTexture = NULL;

	// Open file

	m_inputFile.open(getFileName().c_str(),ios::in);

	// Check file

	if (!m_inputFile)
	{
		IvfDbg1("CIvfAc3DReader: File invalid.");
		return;
	}
	else
		IvfDbg1("CIvfAc3DReader: File ok.");

	// Read and check header

	if (!checkHeader(m_inputFile))
	{
		IvfDbg1("CIvfAc3DReader: Invalid header or unsupported type.");
		m_inputFile.close();
		return;
	}
	else
		IvfDbg1("CIvfAc3DReader: Header ok.");

	// Read data

	if (!readData(m_inputFile))
	{
		IvfDbg1("CIvfAc3DReader: Invalid data.");
		return;
	}
	else
		IvfDbg1("CIvfAc3DReader: data ok.");

	IvfDbg1("CIvfAc3DReader: " << m_surfacesRead << " surfaces read.");

	m_inputFile.close();

	//m_currentPolySet->setUseVertexNormals(true);
}

bool CIvfAc3DReader::checkHeader(istream &in)
{
	getLine(m_row);

	if (find("AC3D", m_row))
	{
		if (find("AC3Db", m_row))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool CIvfAc3DReader::readData(istream &in)
{
	int keyword;
	int optionalKeyword;

	while (!in.eof())
	{
		getLine(m_row);

		using namespace std;

		keyword = getKeyword(m_row);
		optionalKeyword = OKW_ERROR;

		switch (keyword) {
		case KW_OBJECT:
			if (!parseObject(m_row))
				return false;
			break;
		case KW_MATERIAL:
			if (!parseMaterial(m_row))
				return false;
			break;
		case KW_SURF:
			if (!parseSurf(m_row))
				return false;
			break;
		case KW_ERROR:
			optionalKeyword = getOptionalKeyword(m_row);

			switch (optionalKeyword) {
			case OKW_NAME:
				if (!parseName(m_row))
					return false;
				break;
			case OKW_DATA:
				if (!parseData(m_row))
					return false;
				break;
			case OKW_TEXTURE:
				if (!parseTexture(m_row))
					return false;
				break;
			case OKW_ROT:
				if (!parseRot(m_row))
					return false;
				break;
			case OKW_LOC:
				if (!parseLoc(m_row))
					return false;
				break;
			case OKW_URL:
				if (!parseUrl(m_row))
					return false;
				break;
			case OKW_NUMVERT:
				if (!parseNumvert(m_row, in))
					return false;
				break;
			case OKW_NUMSURF:
				if (!parseNumsurf(m_row))
					return false;
				break;
			case OKW_KIDS:
				if (!parseKids(m_row))
					return false;
				break;
			case OKW_MAT:
				if (!parseMat(m_row))
					return false;
				break;
			case OKW_REFS:
				if (!parseRefs(m_row, in))
					return false;
				break;
			case OKW_TEXREP:
				if (!parseTexrep(m_row))
					return false;
				break;
			case OKW_ERROR:

				break;
			default:

				break;
			}
			break;
		default:

			break;
		}

		// We must have a keyword or an optional keyword to continue

		if ((keyword==KW_ERROR)&&(optionalKeyword==OKW_ERROR))
			return false;
	}

	return true;
}

int CIvfAc3DReader::getKeyword(string &row)
{
	if (find("OBJECT", row))
		return KW_OBJECT;

	if (find("MATERIAL", row))
		return KW_MATERIAL;

	if (find("SURF", row))
		return KW_SURF;

	if (row.empty())
		return KW_EMPTY_ROW;

	return KW_ERROR;
}

int CIvfAc3DReader::getOptionalKeyword(string &row)
{
	if (find("name", row))
		return OKW_NAME;

	if (find("data", row))
		return OKW_DATA;

	if (find("texture", row))
		return OKW_TEXTURE;

	if (find("texrep", row))
		return OKW_TEXREP;

	if (find("rot", row))
		return OKW_ROT;

	if (find("loc", row))
		return OKW_LOC;

	if (find("url", row))
		return OKW_URL;

	if (find("mat", row))
		return OKW_MAT;

	if (find("numvert", row))
		return OKW_NUMVERT;

	if (find("numsurf", row))
		return OKW_NUMSURF;

	if (find("kids", row))
		return OKW_KIDS;

	if (find("refs", row))
		return OKW_REFS;

	if (find("texrep", row))
		return OKW_TEXREP;

	return OKW_ERROR;
}

bool CIvfAc3DReader::parseObject(string &row)
{
	using namespace std;

	m_haveKids = false;
	m_haveGeometry = false;

	m_currentPos[0] = 0.0;
	m_currentPos[1] = 0.0;
	m_currentPos[2] = 0.0;

	if (find("world",row))
	{
		IvfDbg1("CIvfAc3DReader: World object found.");
		m_world = new CIvfComposite();
		this->setShape(m_world);
		m_currentGroup = m_world;
		m_currentShape = m_world;
		m_currentObject = OT_WORLD;

		pushGroup(m_currentGroup);
	}
	else if (find("poly", row))
	{
		IvfDbg1("CIvfAc3DReader: Poly object found.");

		m_currentTextureIndex = 0;

		while (!haveMoreChildren())
		{
			popGroup();
			m_currentGroup = currentGroup();
		}
		decChild();

		m_currentLineStripSet = NULL;

		m_currentPolySet = new CIvfPolySet();
		m_currentPolySet->setMaterialSet(m_materialSet);
		m_currentShape = m_currentPolySet;
		m_currentObject = OT_POLY;

	}
	else if (find("group", row))
	{

		IvfDbg1("CIvfAc3DReader: Group object found.");
		m_currentObject = OT_GROUP;

		while (!haveMoreChildren())
		{
			popGroup();
			m_currentGroup = currentGroup();
		}
		decChild();

		CIvfComposite* group = new CIvfComposite();
		m_currentGroup->addChild(group);

		pushGroup(group);

		m_currentGroup = group;
		m_currentShape = group;

	}
	else if (find("light", row))
	{
		m_currentObject = OT_LIGHT;
		m_currentLight = m_lighting->getLight(m_lightCount);
		if (m_lightCount<m_lighting->getSize())
			m_lightCount++;
		else
			m_lightCount = m_lighting->getSize()-1;
		m_currentLight->setType(CIvfLight::LT_POINT);
		m_currentLight->enable();
	}
	else
		return false;

	return true;
}

bool CIvfAc3DReader::parseMaterial(string &row)
{
	int pos1, pos2;
	float fValue1, fValue2, fValue3;

	using namespace std;

	//istringstream strStream(buff,sizeof(buff));

	string params;

	IvfDbg1("CIvfAc3DReader: Found material.");

	//
	// Create material
	//

	CIvfMaterial* material = new CIvfMaterial();
	m_materialSet->addMaterial(material);

	//
	// Read rgb values
	//

	pos1 = row.find("rgb");
	pos2 = row.find("amb");

	if (pos1<0)
		return false;

	params = row.substr(pos1+4,pos2-pos1-4);

	/*
	memset(buff, 0, sizeof(buff));
	strcpy(buff, params.c_str());
	*/

	istringstream strStream;
	strStream.str(params);
	strStream.seekg(0);
	strStream >> fValue1 >> fValue2 >> fValue3;

	IvfDbg1("\trgb  : " << fValue1 << ", " << fValue2 << ", " << fValue3);

	material->setDiffuseColor(fValue1, fValue2, fValue3, 1.0f);

	//
	// Read ambient values
	//

	pos1 = row.find("amb");
	pos2 = row.find("emis");

	if (pos1<0)
		return false;

	params = row.substr(pos1+4, pos2-pos1-4);

	/*
	memset(buff, 0, sizeof(buff));
	strcpy(buff, params.c_str());
	*/

	strStream.str(params);
	strStream.seekg(0);
	strStream >> fValue1 >> fValue2 >> fValue3;

	IvfDbg1("\tamb  : " << fValue1 << ", " << fValue2 << ", " << fValue3);

	material->setAmbientColor(fValue1, fValue2, fValue3, 1.0f);

	//
	// Read emissive values
	//

	pos1 = row.find("emis");
	pos2 = row.find("spec");

	if (pos1<0)
		return false;

	params = row.substr(pos1+5,pos2-pos1-5);

	strStream.str(params);
	strStream.seekg(0);
	strStream >> fValue1 >> fValue2 >> fValue3;

	IvfDbg1("\temis : " << fValue1 << ", " << fValue2 << ", " << fValue3);

	material->setEmissionColor(fValue1, fValue2, fValue3, 1.0f);

	//
	// Read specular values
	//

	pos1 = row.find("spec");
	pos2 = row.find("shi");

	if (pos1<0)
		return false;

	params = row.substr(pos1+5,pos2-pos1-5);

	strStream.str(params);
	strStream.seekg(0);
	strStream >> fValue1 >> fValue2 >> fValue3;

	IvfDbg1("\tspec : " << fValue1 << ", " << fValue2 << ", " << fValue3);

	material->setSpecularColor(fValue1, fValue2, fValue3, 1.0f);

	//
	// Read shininess
	//

	pos1 = row.find("shi");
	pos2 = row.find("trans");

	if (pos1<0)
		return false;

	params = row.substr(pos1+4, pos2-pos1-4);

	strStream.str(params);
	strStream.seekg(0);
	strStream >> fValue1;

	IvfDbg1("\tshi  : " << fValue1);

	material->setShininess(fValue1);

	//
	// Read tranparency
	//

	pos1 = row.find("trans");

	if (pos1<0)
		return false;

	params = row.substr(pos1+6);

	strStream.str(params);
	strStream.seekg(0);
	strStream >> fValue1;

	IvfDbg1("\ttrans : " << fValue1);

	material->setAlphaValue(1.0f-fValue1);

	return true;
}

bool CIvfAc3DReader::parseSurf(string &row)
{
	using namespace std;

	string params;

	istringstream strStream;

	bool poly = false;

	unsigned int flags;

	using namespace std;

	params = row.substr(5);

	strStream.str(params);
	strStream.seekg(0);
	strStream >> flags;

	if (params == "0x22")
		m_surfaceType = ST_POLY_LINE;
	else
		m_surfaceType = ST_POLY;

	//m_surfaceType = (int)(flags & 0xF);
	//m_shadingType =

	m_surfacesRead++;

	return true;
}

bool CIvfAc3DReader::parseName(string &row)
{
	using namespace std;

	string params;
	int pos1, pos2;


	params = row.substr(5);

	pos1 = params.find(34);
	params = params.substr(pos1+1);
	pos2 = params.find(34);
	params = params.substr(0,pos2);

	m_currentName = params;

	if (m_currentObject!=OT_LIGHT)
	{

		IvfDbg1("CIvfAc3DReader: name = " << m_currentName.c_str());

		if (m_currentShape!=NULL)
			m_currentShape->setName(m_currentName.c_str());

		//IvfDbg1("\tName : " <
	}

	return true;
}

bool CIvfAc3DReader::parseTexrep(string &row)
{
	using namespace std;

	string params;
	//istringstream strStream(buff,sizeof(buff));
	istringstream strStream;

	using namespace std;

	params = row.substr(7);

	strStream.str(params);
	strStream.seekg(0);
	strStream >> m_texrep[0] >> m_texrep[1];

	if (m_lastTexture!=NULL)
		m_lastTexture->setTextureModifier(m_texrep[0], m_texrep[1], 0.0);

	return true;
}

bool CIvfAc3DReader::parseData(string &row)
{
	return true;
}

bool CIvfAc3DReader::parseTexture(string &row)
{
	using namespace std;

	string params;
	int pos1, pos2;

	string fileName;
	string prefixDir;
	string temp;
	int findKey = -1;

	fileName = this->getFileName();

	// Check for "/"

	pos1 = fileName.find(47);
	if (pos1>=0)
		findKey = 47;

	// Check for "\"

	pos1 = fileName.find(92);
	if (pos1>=0)
		findKey = 92;

	// Find last separator

	pos1 = fileName.find_last_of(findKey);
	prefixDir = fileName.substr(0,pos1+1);

	params = row.substr(5);

	pos1 = params.find(34);
	params = params.substr(pos1+1);
	pos2 = params.find(34);
	params = params.substr(0,pos2);

	params = prefixDir+params;

	CIvfSgiImage* image = new CIvfSgiImage();
	image->setFileName(params.c_str());
	image->read();

	CIvfTexture* texture = new CIvfTexture();
	texture->setImage(image);
	texture->setTextureMode(GL_MODULATE);
	texture->setGenerateMipmaps(true);
	texture->setFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

	m_currentPolySet->setTexture(texture);

	m_lastTexture = texture;

	IvfDbg1("CIvfAc3DReader: texture = " << params.c_str());

	return true;
}

bool CIvfAc3DReader::parseRot(string &row)
{
	return true;
}

bool CIvfAc3DReader::parseLoc(string &row)
{
	using namespace std;

	string params;
	//istringstream strStream(buff,sizeof(buff));
	istringstream strStream;

	using namespace std;

	params = row.substr(4);

	strStream.str(params);
	strStream.seekg(0);
	strStream >> m_currentPos[0] >> m_currentPos[1] >> m_currentPos[2];

	return true;
}

bool CIvfAc3DReader::parseUrl(string &row)
{
	return true;
}

bool CIvfAc3DReader::parseNumvert(string &row, istream &in)
{
	using namespace std;

	string params;
	istringstream strStream;

	int i, nVerts;
	double x, y, z;

	using namespace std;

	params = row.substr(8);

	strStream.str(params);
	strStream.seekg(0);
	strStream >> nVerts;

	for (i=0; i<nVerts; i++)
	{
		in >> x >> y >> z;
		m_currentPolySet->addCoord(x, y, z);
	}

	IvfDbg1("CIvfAc3DReader: " << nVerts << " vertices read.");

	m_haveGeometry = true;

	return true;
}

bool CIvfAc3DReader::parseNumsurf(string &row)
{
	using namespace std;

	string params;
	istringstream strStream;

	params = row.substr(8);

	strStream.str(params);
	strStream.seekg(0);
	strStream >> m_nSurfaces;

	return true;
}

bool CIvfAc3DReader::parseMat(string &row)
{
	using namespace std;

	string params;
	istringstream strStream;

	int materialIdx;

	params = row.substr(4);

	strStream.str(params);
	strStream.seekg(0);
	strStream >> materialIdx;

	m_currentMaterialIdx = materialIdx;

	if ((materialIdx>=0)&&(materialIdx<m_materialSet->getSize()))
		m_currentMaterial = m_materialSet->getMaterial(materialIdx);
	else
		m_currentMaterial = NULL;

	return true;
}

bool CIvfAc3DReader::parseRefs(string &row, istream &in)
{
	using namespace std;

	string params;
	istringstream strStream;

	int i, coordIdx, nVertices;
	double s, t;
	double x, y, z;

	CIvfIndex* textureIndex;
	CIvfIndex* idx;

	params = row.substr(5);

	strStream.str(params);
	strStream.seekg(0);
	strStream >> nVertices;

	if (m_surfaceType==ST_POLY)
	{
		idx = new CIvfIndex();
		textureIndex = new CIvfIndex();
	}
	else
	{
		// Default shape is a CIvfPolySet. Now we want a
		// CIvfLineStripSet, so we have to convert the
		// polyset to a line strip set.

		idx = new CIvfIndex();

		m_currentLineStripSet = new CIvfLineStripSet();
		m_currentShape = m_currentLineStripSet;

		m_currentLineStripSet->setName(m_currentPolySet->getName());

		for (i=0; i<m_currentPolySet->getCoordSetSize(); i++)
		{
			m_currentPolySet->getCoord(i, x, y, z);
			m_currentLineStripSet->addCoord(x, y, z);
		}

		//delete m_currentPolySet;
		//m_currentPolySet = NULL;
	}

	if (m_surfaceType == ST_POLY)
	{
		for (i=0; i<nVertices; i++)
		{
			in >> coordIdx >> s >> t;

			m_currentPolySet->addTextureCoord(s, t);
			textureIndex->add(m_currentTextureIndex);
			m_currentTextureIndex++;

			idx->add(coordIdx);
		}

		if ((nVertices>2)&&(nVertices<5))
		{
			if (nVertices==3)
				idx->setTopology(IVF_IDX_TRIANGLES);
			else
				idx->setTopology(IVF_IDX_QUADS);

			m_currentPolySet->addCoordIndex(idx);
			m_currentPolySet->addTextureIndex(textureIndex);
			m_currentPolySet->addMaterialIndex(m_currentMaterialIdx);
		}
		else
			IvfDbg1("CIvfAc3DReader: Topology not supported. (refs " << nVertices << " )");
	}
	else
	{
		// Read line strip

		for (i=0; i<nVertices; i++)
		{
			in >> coordIdx >> s >> t;
			idx->add(coordIdx);
		}

		idx->add(coordIdx);

		m_currentLineStripSet->addCoordIndex(idx);
		m_currentLineStripSet->addMaterialIndex(m_currentMaterialIdx);
	}

	return true;
}

bool CIvfAc3DReader::parseKids(string &row)
{
	using namespace std;

	string params;
	istringstream strStream;

	params = row.substr(5);

	strStream.str(params);
	strStream.seekg(0);
	strStream >> m_currentNumberOfKids;


	if (m_currentNumberOfKids!=0)
	{

		// A poly can be a parent too

		if (m_currentObject == OT_POLY)
		{
			CIvfComposite* group = new CIvfComposite();
			m_currentGroup->addChild(group);

			pushGroup(group);

			m_currentGroup = group;
			m_currentShape = group;

			//delete m_currentPolySet;
			//IvfDbg1("poly, kids = " << m_currentNumberOfKids);

		}
		else
		{
			//IvfDbg1("group, kids = " << m_currentNumberOfKids);
		}

		m_childStack[m_currentLevel] = m_currentNumberOfKids;

		if (m_currentGroup->getTag()<0)
		{
			//IvfDbg1("loc = " << m_currentPos[0] << ", " << m_currentPos[1] << ", " << m_currentPos[2]);
			m_currentGroup->setPosition(m_currentPos[0], m_currentPos[1], m_currentPos[2]);
			m_currentGroup->setTag(1);
		}

	}
	else
	{
		// If we have a standard poly object add it to
		// current group

		if (m_currentObject == OT_POLY)
		{
			// m_currentGroup->addChild(m_currentPolySet);
			m_currentGroup->addChild(m_currentShape);
			//IvfDbg1("loc = " << m_currentPos[0] << ", " << m_currentPos[1] << ", " << m_currentPos[2]);
			m_currentShape->setPosition(m_currentPos[0], m_currentPos[1], m_currentPos[2]);
		}

		if (m_currentObject == OT_LIGHT)
		{
			m_currentLight->setPosition(m_currentPos[0], m_currentPos[1], m_currentPos[2]);
		}
	}

	m_haveKids = true;

	return true;
}

void CIvfAc3DReader::clearVertexList()
{
	//int i;

	m_vertexList.clear();

	/*
	for (i=0; i<m_vertexList.size(); i++)
	{
		CIvfVec3d* vec3d = m_vertexList[i];
		delete vec3d;
	}
	*/
}

void CIvfAc3DReader::pushGroup(CIvfComposite *group)
{
	//IvfDbg1("-->");
	if (m_groupStack[m_currentLevel]==NULL)
		m_groupStack[m_currentLevel] = group;
	else
	{
		m_currentLevel++;
		m_groupStack[m_currentLevel] = group;
	}
}

void CIvfAc3DReader::popGroup()
{
	//IvfDbg1("<--");
	m_groupStack[m_currentLevel]=NULL;

	if (m_currentLevel>0)
		m_currentLevel--;
}

CIvfComposite* CIvfAc3DReader::currentGroup()
{
	return m_groupStack[m_currentLevel];
}

void CIvfAc3DReader::decChild()
{
	m_childStack[m_currentLevel]--;
}

bool CIvfAc3DReader::haveMoreChildren()
{
	return m_childStack[m_currentLevel]>0;
}

void CIvfAc3DReader::initGroupStack()
{
	m_currentGroup = m_world;
	m_currentLevel = 0;
	m_childStack[m_currentLevel] = 0;

	int i;

	for (i=0; i<100; i++)
	{
		m_groupStack[i] = NULL;
		m_childStack[i] = 0;
	}
}


