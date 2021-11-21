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

#include <ivffile/Ac3DReader.h>

#include <ivfimage/PngImage.h>

#include <ivf/Texture.h>

#include <sstream>

using namespace ivf;

#define IvfLog(txt) \
	std::cout << txt << std::endl;

#define KW_ERROR       -1
#define KW_MATERIAL     0
#define KW_OBJECT       1
#define KW_SURF         2
#define KW_EMPTY_ROW	3
#define KW_COMMENT		4

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
#define OKW_CREASE		12

using namespace std;

CAc3DReader::CAc3DReader()
{
	m_currentPolySet = nullptr;
	m_materialSet = new CMaterialSet();
	//m_materialSet->addReference();

	m_lighting = CLighting::getInstance();
	m_lightCount = 0;
}

CAc3DReader::~CAc3DReader()
{
	//m_materialSet->deleteReference();
}

void CAc3DReader::read()
{
	// Initiate max min calc

	resetSize();

	// Initialize variables

	initGroupStack();
	m_surfacesRead = 0;
	m_currentName = "noname";

	m_texrep[0] = 1.0;
	m_texrep[1] = 1.0;

	m_currentLight = nullptr;

	m_lastTexture = nullptr;

	// Open file

	m_inputFile.open(getFileName().c_str(),ios::in);

	// Check file

	if (!m_inputFile)
	{
		IvfLog("CAc3DReader: File invalid.");
		return;
	}
	else
		IvfLog("CAc3DReader: File ok.");

	// Read and check header

	if (!checkHeader(m_inputFile))
	{
		IvfLog("CAc3DReader: Invalid header or unsupported type.");
		m_inputFile.close();
		return;
	}
	else
		IvfLog("CAc3DReader: Header ok.");

	// Read data

	if (!readData(m_inputFile))
	{
		IvfLog("CAc3DReader: Invalid data.");
		return;
	}
	else
		IvfLog("CAc3DReader: data ok.");

	IvfLog("CAc3DReader: " << m_surfacesRead << " surfaces read.");

	m_inputFile.close();

	//m_currentPolySet->setUseVertexNormals(true);
}

bool CAc3DReader::checkHeader(istream &in)
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

bool CAc3DReader::readData(istream &in)
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
		case KW_COMMENT:
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

int CAc3DReader::getKeyword(string &row)
{
	if (find("OBJECT", row))
		return KW_OBJECT;

	if (find("MATERIAL", row))
		return KW_MATERIAL;

	if (find("SURF", row))
		return KW_SURF;

	if (find("//", row))
		return KW_COMMENT;

	if (row.empty())
		return KW_EMPTY_ROW;

	return KW_ERROR;
}

int CAc3DReader::getOptionalKeyword(string &row)
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

	if (find("crease", row))
		return OKW_CREASE;

	return OKW_ERROR;
}

bool CAc3DReader::parseObject(string &row)
{
	using namespace std;

	m_haveKids = false;
	m_haveGeometry = false;

	m_currentPos[0] = 0.0;
	m_currentPos[1] = 0.0;
	m_currentPos[2] = 0.0;

	if (find("world",row))
	{
		IvfLog("CAc3DReader: World object found.");
		m_world = new CComposite();
		this->setShape(m_world);
		m_currentGroup = m_world;
		m_currentShape = m_world;
		m_currentObject = OT_WORLD;

		pushGroup(m_currentGroup);
	}
	else if (find("poly", row))
	{
		IvfLog("CAc3DReader: Poly object found.");

		m_currentTextureIndex = 0;

		while (!haveMoreChildren())
		{
			popGroup();
			m_currentGroup = currentGroup();
		}
		decChild();

		m_currentLineStripSet = nullptr;

		m_currentPolySet = new CPolySet();
		//m_currentPolySet->setUseVertexNormals(true);
		m_currentPolySet->setMaterialSet(m_materialSet);
		m_currentShape = m_currentPolySet;
		m_currentObject = OT_POLY;

	}
	else if (find("group", row))
	{

		IvfLog("CAc3DReader: Group object found.");
		m_currentObject = OT_GROUP;

		while (!haveMoreChildren())
		{
			popGroup();
			m_currentGroup = currentGroup();
		}
		decChild();

		CComposite* group = new CComposite();
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
		m_currentLight->setType(CLight::LT_POINT);
		m_currentLight->enable();
	}
	else
		return false;

	return true;
}

bool CAc3DReader::parseMaterial(string &row)
{
	int pos1, pos2;
	float fValue1, fValue2, fValue3;

	using namespace std;

	//istringstream strStream(buff,sizeof(buff));

	string params;

	IvfLog("CAc3DReader: Found material.");

	//
	// Create material
	//

	CMaterial* material = new CMaterial();
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

	IvfLog("\trgb  : " << fValue1 << ", " << fValue2 << ", " << fValue3);

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

	IvfLog("\tamb  : " << fValue1 << ", " << fValue2 << ", " << fValue3);

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

	IvfLog("\temis : " << fValue1 << ", " << fValue2 << ", " << fValue3);

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

	IvfLog("\tspec : " << fValue1 << ", " << fValue2 << ", " << fValue3);

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

	IvfLog("\tshi  : " << fValue1);

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

	IvfLog("\ttrans : " << fValue1);

	material->setAlphaValue(1.0f-fValue1);

	return true;
}

bool CAc3DReader::parseSurf(string &row)
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

	if (params == "0x32")
		m_surfaceType = ST_POLY_LINE;
	else
		m_surfaceType = ST_POLY;

	//m_surfaceType = (int)(flags & 0xF);
	//m_shadingType =

	m_surfacesRead++;  

	return true;
}

bool CAc3DReader::parseName(string &row)
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

		IvfLog("CAc3DReader: name = " << m_currentName.c_str());

		if (m_currentShape!=nullptr)
			m_currentShape->setName(m_currentName.c_str());

		//IvfLog("\tName : " <
	}

	return true;
}

bool CAc3DReader::parseTexrep(string &row)
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

	if (m_lastTexture!=nullptr)
		m_lastTexture->setTextureModifier(m_texrep[0], m_texrep[1], 0.0);

	return true;
}

bool CAc3DReader::parseData(string &row)
{
	return true;
}

bool CAc3DReader::parseTexture(string &row)
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

	CPngImage* image = new CPngImage();
	image->setFileName(params.c_str());
	image->read();

	CTexture* texture = new CTexture();
	texture->setImage(image);
	texture->setTextureMode(GL_MODULATE);
	texture->setGenerateMipmaps(true);
	texture->setFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

	m_currentPolySet->setTexture(texture);

	m_lastTexture = texture;

	IvfLog("CAc3DReader: texture = " << params.c_str());

	return true;
}

bool CAc3DReader::parseRot(string &row)
{
	return true;
}

bool CAc3DReader::parseLoc(string &row)
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

bool CAc3DReader::parseUrl(string &row)
{
	return true;
}

bool CAc3DReader::parseNumvert(string &row, istream &in)
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

	IvfLog("CAc3DReader: " << nVerts << " vertices read.");

	m_haveGeometry = true;

	return true;
}

bool CAc3DReader::parseNumsurf(string &row)
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

bool CAc3DReader::parseMat(string &row)
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
		m_currentMaterial = nullptr;

	return true;
}

bool CAc3DReader::parseRefs(string &row, istream &in)
{
	using namespace std;

	string params;
	istringstream strStream;

	int i, coordIdx, nVertices;
	double s, t;
	double x, y, z;

	CIndex* textureIndex;
	CIndex* idx;

	params = row.substr(5);

	strStream.str(params);
	strStream.seekg(0);
	strStream >> nVertices;

	if (m_surfaceType==ST_POLY)
	{
		idx = new CIndex();
		textureIndex = new CIndex();
	}
	else
	{
		// Default shape is a CPolySet. Now we want a
		// CLineStripSet, so we have to convert the
		// polyset to a line strip set.

		idx = new CIndex();

		m_currentLineStripSet = new CLineStripSet();
		m_currentShape = m_currentLineStripSet;

		m_currentLineStripSet->setName(m_currentPolySet->getName());

		for (i=0; i<m_currentPolySet->getCoordSetSize(); i++)
		{
			m_currentPolySet->getCoord(i, x, y, z);
			m_currentLineStripSet->addCoord(x, y, z);
		}

		//delete m_currentPolySet;
		//m_currentPolySet = nullptr;
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
			IvfLog("CAc3DReader: Topology not supported. (refs " << nVertices << " )");
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

bool CAc3DReader::parseKids(string &row)
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
			CComposite* group = new CComposite();
			m_currentGroup->addChild(group);

			pushGroup(group);

			m_currentGroup = group;
			m_currentShape = group;

			//delete m_currentPolySet;
			//IvfLog("poly, kids = " << m_currentNumberOfKids);

		}
		else
		{
			//IvfLog("group, kids = " << m_currentNumberOfKids);
		}

		m_childStack[m_currentLevel] = m_currentNumberOfKids;

		if (m_currentGroup->getTag()<0)
		{
			//IvfLog("loc = " << m_currentPos[0] << ", " << m_currentPos[1] << ", " << m_currentPos[2]);
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
			m_currentShape->refresh();
			//IvfLog("loc = " << m_currentPos[0] << ", " << m_currentPos[1] << ", " << m_currentPos[2]);
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

void CAc3DReader::clearVertexList()
{
	//int i;

	m_vertexList.clear();

	/*
	for (i=0; i<m_vertexList.size(); i++)
	{
		CVec3d* vec3d = m_vertexList[i];
		delete vec3d;
	}
	*/
}

void CAc3DReader::pushGroup(CComposite *group)
{
	//IvfLog("-->");
	if (m_groupStack[m_currentLevel]==nullptr)
		m_groupStack[m_currentLevel] = group;
	else
	{
		m_currentLevel++;
		m_groupStack[m_currentLevel] = group;
	}
}

void CAc3DReader::popGroup()
{
	//IvfLog("<--");
	m_groupStack[m_currentLevel]=nullptr;

	if (m_currentLevel>0)
		m_currentLevel--;
}

CComposite* CAc3DReader::currentGroup()
{
	return m_groupStack[m_currentLevel];
}

void CAc3DReader::decChild()
{
	m_childStack[m_currentLevel]--;
}

bool CAc3DReader::haveMoreChildren()
{
	return m_childStack[m_currentLevel]>0;
}

void CAc3DReader::initGroupStack()
{
	m_currentGroup = m_world;
	m_currentLevel = 0;
	m_childStack[m_currentLevel] = 0;

	int i;

	for (i=0; i<100; i++)
	{
		m_groupStack[i] = nullptr;
		m_childStack[i] = 0;
	}
}


