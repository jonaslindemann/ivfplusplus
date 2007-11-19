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

#include <ivffile/IvfDxfWriter.h>

#include <ivf/IvfComposite.h>
#include <ivf/IvfQuadSet.h>
#include <ivf/IvfTriSet.h>
#include <ivf/IvfTriStripSet.h>
#include <ivf/IvfPolySet.h>

using namespace std;

CIvfDxfWriter::CIvfDxfWriter()
{
	m_matrixStack = new CIvfMatrixStack();	
	m_currentLayer = "0";
	m_currentColor = 256;
}

CIvfDxfWriter::~CIvfDxfWriter()
{
	
}

void CIvfDxfWriter::dxfTag(int number, const char* value)
{
	m_file << "  " << number << endl;
	m_file << value << endl;
}

void CIvfDxfWriter::dxfInt(int number, int value)
{
    m_file << "  " << number << endl;
    m_file << value << endl;	
}

void CIvfDxfWriter::dxfFloat(int number, double value)
{
    m_file << "  " << number << endl;
    m_file << value << endl;
}

void CIvfDxfWriter::dxfBeginSection(const char* value)
{
	dxfTag(0, "SECTION");
	dxfTag(2, value);	
}

void CIvfDxfWriter::dxfEndSection()
{
	dxfTag(0, "ENDSEC");
}

void CIvfDxfWriter::dxfBeginTables()
{
	dxfBeginSection("TABLES");
}

void CIvfDxfWriter::dxfEndTables()
{
	dxfEndSection();
}

void CIvfDxfWriter::dxfBeginTable(const char* value)
{
	dxfTag(0, "TABLE");
	dxfTag(2, value);
}

void CIvfDxfWriter::dxfEndTable()
{
	dxfTag(0, "ENDTAB");
}

void CIvfDxfWriter::dxfBeginLayer()
{
	dxfBeginTable("LAYER");
}

void CIvfDxfWriter::dxfEndLayer()
{
	dxfEndTable();	
}

void CIvfDxfWriter::dxfBeginEntitites()
{
	dxfBeginSection("ENTITIES");	
}

void CIvfDxfWriter::dxfEndEntitites()
{
	dxfEndSection();	
}

void CIvfDxfWriter::dxfColor(int number)
{
	dxfInt(62, number);	
}

void CIvfDxfWriter::dxfLineType(const char* name)
{
	
}

void CIvfDxfWriter::dxfLine()
{
	dxfTag(0, "LINE");	
}

void CIvfDxfWriter::dxfLayer(const char* value)
{
	dxfTag(8, value);
}

void CIvfDxfWriter::dxfFirstPoint(double x, double y, double z)
{
	dxfFloat(10, x);
	dxfFloat(20, y);
	dxfFloat(30, z);
}

void CIvfDxfWriter::dxfSecondPoint(double x, double y, double z)
{
	dxfFloat(11, x);
	dxfFloat(21, y);	
	dxfFloat(31, z);	
}

void CIvfDxfWriter::dxfThirdPoint(double x, double y, double z)
{
	dxfFloat(12, x);
	dxfFloat(22, y);	
	dxfFloat(32, z);	
}

void CIvfDxfWriter::dxfFourthPoint(double x, double y, double z)
{
	dxfFloat(13, x);
	dxfFloat(23, y);	
	dxfFloat(33, z);	
}

void CIvfDxfWriter::dxfPolyline()
{
	dxfTag(0, "POLYLINE");	
}

void CIvfDxfWriter::dxfPolylineIntro()
{
	dxfInt(66, 1);
	dxfFloat(10, 0.0);
	dxfFloat(20, 0.0);
	dxfFloat(30, 0.0);
	dxfPolylineFlag(8);	
}

void CIvfDxfWriter::dxfPolylineFlag(int flag)
{
	dxfInt(70, flag);	
}

void CIvfDxfWriter::dxfVertex()
{
	dxfTag(0, "VERTEX");	
}

void CIvfDxfWriter::dxfEndSeq()
{
	dxfTag(0, "SEQEND");
}

void CIvfDxfWriter::dxfSolid()
{
	dxfTag(0, "SOLID");	
}

void CIvfDxfWriter::dxfEndOfFile()
{
	dxfTag(0, "EOF");
}

void CIvfDxfWriter::dxf3DFace()
{
	dxfTag(0, "3DFACE");
}

void CIvfDxfWriter::quad(
		double x1, double y1, double z1,
		double x2, double y2, double z2,
		double x3, double y3, double z3,
		double x4, double y4, double z4
		)
{
	dxf3DFace();
	dxfLayer(m_currentLayer.c_str());
	dxfColor(m_currentColor);
	transform(x1, y1, z1);
	dxfFirstPoint(x1, y1, z1);
	transform(x2, y2, z2);
	dxfSecondPoint(x2, y2, z2);
	transform(x3, y3, z3);
	dxfThirdPoint(x3, y3, z3);
	transform(x4, y4, z4);
	dxfFourthPoint(x4, y4, z4);
}

void CIvfDxfWriter::tri(
		double x1, double y1, double z1,
		double x2, double y2, double z2,
		double x3, double y3, double z3
		)
{
	dxf3DFace();
	dxfLayer(m_currentLayer.c_str());
	dxfColor(m_currentColor);
	transform(x1, y1, z1);
	dxfFirstPoint(x1, y1, z1);
	transform(x2, y2, z2);
	dxfSecondPoint(x2, y2, z2);
	transform(x3, y3, z3);
	dxfThirdPoint(x3, y3, z3);
	dxfFourthPoint(x3, y3, z3);
}

void CIvfDxfWriter::pushMatrix()
{
	m_matrixStack->pushMatrix();
}

void CIvfDxfWriter::popMatrix()
{
	m_matrixStack->popMatrix();
}

void CIvfDxfWriter::transform(double &x, double &y, double &z)
{
	m_matrixStack->getWorldCoordinate(x, y, z, x, y, z);
}

void CIvfDxfWriter::translate(double dx, double dy, double dz)
{
	m_matrixStack->translate(dx, dy, dz);
}

void CIvfDxfWriter::rotate(double vx, double vy, double vz, double theta)
{
	m_matrixStack->rotate(vx, vy, vz, theta);
}

void CIvfDxfWriter::setCurrentLayer(const char* layerName)
{
	m_currentLayer = layerName;
}

void CIvfDxfWriter::setCurrentColor(int color)
{
	m_currentColor = color;
}


void CIvfDxfWriter::write()
{
	CIvfShape* shape = this->getShape();

	m_file.open(this->getFileName(), ios::out);

	if (m_file.good())
	{
		dxfBeginEntitites();	
		
		processShape(shape);
		
		dxfEndEntitites();
		dxfEndOfFile();
		
		m_file.close();
	}
}

void CIvfDxfWriter::processShape(CIvfShape *shape)
{
	if (shape->getName()!=NULL)
		setCurrentLayer(shape->getName());

	cout << "shape = " << shape->getClassName() << endl;

	if (shape->isClass("CIvfComposite")) 
	{
		int i;
		double x, y, z;
		double vx, vy, vz, angle;

		cout << "processShape: Found " << shape->getClassName() << endl;

		CIvfCompositePtr composite = (CIvfComposite*)shape;
		composite->getPosition(x, y, z);
		composite->getRotationQuat(vx, vy, vz, angle);

		pushMatrix();
		translate(x, y, z);
		rotate(vx, vy, vz, angle);
		
		for (i = 0; i<composite->getSize(); i++)
		{
			CIvfShapePtr child = composite->getChild(i);
			processShape(child);
		}

		popMatrix();
	}
	else
	{
		int i, j;
		double x1, y1, z1;
		double x2, y2, z2;
		double x3, y3, z3;
		double x4, y4, z4;
		double x, y, z;
		double vx, vy, vz, angle;
		
		if (shape->isClass("CIvfQuadSet"))
		{
			cout << "processShape: Exporting " << shape->getClassName() << endl;
			CIvfQuadSetPtr quadSet = (CIvfQuadSet*) shape;

			quadSet->getPosition(x, y, z);
			quadSet->getRotationQuat(vx, vy, vz, angle);

			pushMatrix();
			translate(x, y, z);
			rotate(vx, vy, vz, angle);

			for (i=0; i<quadSet->getCoordIndexSetSize(); i++)
			{
				CIvfIndexPtr index = quadSet->getCoordIndex(i);
				
				for (j=0; j<index->getSize(); j+=4)
				{
					quadSet->getCoord(index->getIndex(j), x1, y1, z1);
					quadSet->getCoord(index->getIndex(j+1), x2, y2, z2);
					quadSet->getCoord(index->getIndex(j+2), x3, y3, z3);
					quadSet->getCoord(index->getIndex(j+3), x4, y4, z4);
					quad(
						x1, y1, z1,
						x2, y2, z2,
						x3, y3, z3,
						x4, y4, z4
						);
				}
			}
			popMatrix();
		}

		if (shape->isClass("CIvfTriSet"))
		{
			cout << "processShape: Exporting " << shape->getClassName() << endl;
			CIvfTriSetPtr triSet = (CIvfTriSet*) shape;

			triSet->getPosition(x, y, z);
			triSet->getRotationQuat(vx, vy, vz, angle);

			pushMatrix();
			translate(x, y, z);
			rotate(vx, vy, vz, angle);

			for (i=0; i<triSet->getCoordIndexSetSize(); i++)
			{
				CIvfIndexPtr index = triSet->getCoordIndex(i);
				
				for (j=0; j<index->getSize(); j+=3)
				{
					triSet->getCoord(index->getIndex(j), x1, y1, z1);
					triSet->getCoord(index->getIndex(j+1), x2, y2, z2);
					triSet->getCoord(index->getIndex(j+2), x3, y3, z3);
					tri(
						x1, y1, z1,
						x2, y2, z2,
						x3, y3, z3
						);
				}
			}
			popMatrix();
		}

		if (shape->isClass("CIvfTriStripSet"))
		{
			cout << "processShape: Exporting " << shape->getClassName() << endl;
			CIvfTriStripSetPtr triSet = (CIvfTriStripSet*) shape;

			triSet->getPosition(x, y, z);
			triSet->getRotationQuat(vx, vy, vz, angle);

			pushMatrix();
			translate(x, y, z);
			rotate(vx, vy, vz, angle);

			for (i=0; i<triSet->getCoordIndexSetSize(); i++)
			{
				CIvfIndexPtr index = triSet->getCoordIndex(i);
				
				for (j=0; j<index->getSize()-2; j++)
				{
					triSet->getCoord(index->getIndex(j), x1, y1, z1);
					triSet->getCoord(index->getIndex(j+1), x2, y2, z2);
					triSet->getCoord(index->getIndex(j+2), x3, y3, z3);
					tri(
						x1, y1, z1,
						x2, y2, z2,
						x3, y3, z3
						);
				}
			}
			popMatrix();
		}

		if (shape->isClass("CIvfPolySet"))
		{
			cout << "processShape: Exporting " << shape->getClassName() << endl;
			CIvfPolySetPtr polySet = (CIvfPolySet*) shape;

			polySet->getPosition(x, y, z);
			polySet->getRotationQuat(vx, vy, vz, angle);

			pushMatrix();
			translate(x, y, z);
			rotate(vx, vy, vz, angle);

			for (i=0; i<polySet->getCoordIndexSetSize(); i++)
			{
				CIvfIndexPtr index = polySet->getCoordIndex(i);
				if (index->getTopology()==IVF_IDX_TRIANGLES)
				{
					polySet->getCoord(index->getIndex(0), x1, y1, z1);
					polySet->getCoord(index->getIndex(1), x2, y2, z2);
					polySet->getCoord(index->getIndex(2), x3, y3, z3);
				
					tri(
						x1, y1, z1,
						x2, y2, z2,
						x3, y3, z3
					);
				}
				else
				{
					polySet->getCoord(index->getIndex(0), x1, y1, z1);
					polySet->getCoord(index->getIndex(1), x2, y2, z2);
					polySet->getCoord(index->getIndex(2), x3, y3, z3);
					polySet->getCoord(index->getIndex(3), x4, y4, z4);
				
					quad(
						x1, y1, z1,
						x2, y2, z2,
						x3, y3, z3,
						x4, y4, z4
					);
				}
			}
			popMatrix();
		}
	}
}
