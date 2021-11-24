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

#include <ivffile/DxfWriter.h>

#include <ivf/Composite.h>
#include <ivf/QuadSet.h>
#include <ivf/TriSet.h>
#include <ivf/TriStripSet.h>
#include <ivf/PolySet.h>

using namespace ivf;
using namespace std;

DxfWriter::DxfWriter()
{
	m_matrixStack = new MatrixStack();
	m_currentLayer = "0";
	m_currentColor = 256;
}

DxfWriter::~DxfWriter()
{

}

void DxfWriter::dxfTag(int number, const std::string& value)
{
	m_file << "  " << number << endl;
	m_file << value << endl;
}

void DxfWriter::dxfInt(int number, int value)
{
    m_file << "  " << number << endl;
    m_file << value << endl;
}

void DxfWriter::dxfFloat(int number, double value)
{
    m_file << "  " << number << endl;
    m_file << value << endl;
}

void DxfWriter::dxfBeginSection(const std::string& value)
{
	dxfTag(0, "SECTION");
	dxfTag(2, value);
}

void DxfWriter::dxfEndSection()
{
	dxfTag(0, "ENDSEC");
}

void DxfWriter::dxfBeginTables()
{
	dxfBeginSection("TABLES");
}

void DxfWriter::dxfEndTables()
{
	dxfEndSection();
}

void DxfWriter::dxfBeginTable(const std::string& value)
{
	dxfTag(0, "TABLE");
	dxfTag(2, value);
}

void DxfWriter::dxfEndTable()
{
	dxfTag(0, "ENDTAB");
}

void DxfWriter::dxfBeginLayer()
{
	dxfBeginTable("LAYER");
}

void DxfWriter::dxfEndLayer()
{
	dxfEndTable();
}

void DxfWriter::dxfBeginEntitites()
{
	dxfBeginSection("ENTITIES");
}

void DxfWriter::dxfEndEntitites()
{
	dxfEndSection();
}

void DxfWriter::dxfColor(int number)
{
	dxfInt(62, number);
}

void DxfWriter::dxfLineType(const std::string& name)
{

}

void DxfWriter::dxfLine()
{
	dxfTag(0, "LINE");
}

void DxfWriter::dxfLayer(const std::string& value)
{
	dxfTag(8, value);
}

void DxfWriter::dxfFirstPoint(double x, double y, double z)
{
	dxfFloat(10, x);
	dxfFloat(20, y);
	dxfFloat(30, z);
}

void DxfWriter::dxfSecondPoint(double x, double y, double z)
{
	dxfFloat(11, x);
	dxfFloat(21, y);
	dxfFloat(31, z);
}

void DxfWriter::dxfThirdPoint(double x, double y, double z)
{
	dxfFloat(12, x);
	dxfFloat(22, y);
	dxfFloat(32, z);
}

void DxfWriter::dxfFourthPoint(double x, double y, double z)
{
	dxfFloat(13, x);
	dxfFloat(23, y);
	dxfFloat(33, z);
}

void DxfWriter::dxfPolyline()
{
	dxfTag(0, "POLYLINE");
}

void DxfWriter::dxfPolylineIntro()
{
	dxfInt(66, 1);
	dxfFloat(10, 0.0);
	dxfFloat(20, 0.0);
	dxfFloat(30, 0.0);
	dxfPolylineFlag(8);
}

void DxfWriter::dxfPolylineFlag(int flag)
{
	dxfInt(70, flag);
}

void DxfWriter::dxfVertex()
{
	dxfTag(0, "VERTEX");
}

void DxfWriter::dxfEndSeq()
{
	dxfTag(0, "SEQEND");
}

void DxfWriter::dxfSolid()
{
	dxfTag(0, "SOLID");
}

void DxfWriter::dxfEndOfFile()
{
	dxfTag(0, "EOF");
}

void DxfWriter::dxf3DFace()
{
	dxfTag(0, "3DFACE");
}

void DxfWriter::quad(
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

void DxfWriter::tri(
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

void DxfWriter::pushMatrix()
{
	m_matrixStack->pushMatrix();
}

void DxfWriter::popMatrix()
{
	m_matrixStack->popMatrix();
}

void DxfWriter::transform(double &x, double &y, double &z)
{
	m_matrixStack->getWorldCoordinate(x, y, z, x, y, z);
}

void DxfWriter::translate(double dx, double dy, double dz)
{
	m_matrixStack->translate(dx, dy, dz);
}

void DxfWriter::rotate(double vx, double vy, double vz, double theta)
{
	m_matrixStack->rotate(vx, vy, vz, theta);
}

void DxfWriter::setCurrentLayer(const std::string& layerName)
{
	m_currentLayer = layerName;
}

void DxfWriter::setCurrentColor(int color)
{
	m_currentColor = color;
}


void DxfWriter::write()
{
	Shape* shape = this->getShape();

	m_file.open(this->getFileName().c_str(), ios::out);

	if (m_file.good())
	{
		dxfBeginEntitites();

		processShape(shape);

		dxfEndEntitites();
		dxfEndOfFile();

		m_file.close();
	}
}

void DxfWriter::processShape(Shape *shape)
{
	if (shape->getName()!="")
		setCurrentLayer(shape->getName());

	cout << "shape = " << shape->getClassName() << endl;

	if (shape->isClass("CIvfComposite"))
	{
		int i;
		double x, y, z;
		double vx, vy, vz, angle;

		cout << "processShape: Found " << shape->getClassName() << endl;

		CompositePtr composite = (Composite*)shape;
		composite->getPosition(x, y, z);
		composite->getRotationQuat(vx, vy, vz, angle);

		pushMatrix();
		translate(x, y, z);
		rotate(vx, vy, vz, angle);

		for (i = 0; i<composite->getSize(); i++)
		{
			ShapePtr child = composite->getChild(i);
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

		if (shape->isClass("QuadSet"))
		{
			cout << "processShape: Exporting " << shape->getClassName() << endl;
			QuadSetPtr quadSet = (QuadSet*) shape;

			quadSet->getPosition(x, y, z);
			quadSet->getRotationQuat(vx, vy, vz, angle);

			pushMatrix();
			translate(x, y, z);
			rotate(vx, vy, vz, angle);

			for (i=0; i<quadSet->getCoordIndexSetSize(); i++)
			{
				IndexPtr index = quadSet->getCoordIndex(i);

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

		if (shape->isClass("TriSet"))
		{
			cout << "processShape: Exporting " << shape->getClassName() << endl;
			TriSetPtr triSet = (TriSet*) shape;

			triSet->getPosition(x, y, z);
			triSet->getRotationQuat(vx, vy, vz, angle);

			pushMatrix();
			translate(x, y, z);
			rotate(vx, vy, vz, angle);

			for (i=0; i<triSet->getCoordIndexSetSize(); i++)
			{
				IndexPtr index = triSet->getCoordIndex(i);

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

		if (shape->isClass("TriStripSet"))
		{
			cout << "processShape: Exporting " << shape->getClassName() << endl;
			TriStripSetPtr triSet = (TriStripSet*) shape;

			triSet->getPosition(x, y, z);
			triSet->getRotationQuat(vx, vy, vz, angle);

			pushMatrix();
			translate(x, y, z);
			rotate(vx, vy, vz, angle);

			for (i=0; i<triSet->getCoordIndexSetSize(); i++)
			{
				IndexPtr index = triSet->getCoordIndex(i);

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

		if (shape->isClass("PolySet"))
		{
			cout << "processShape: Exporting " << shape->getClassName() << endl;
			PolySetPtr polySet = (PolySet*) shape;

			polySet->getPosition(x, y, z);
			polySet->getRotationQuat(vx, vy, vz, angle);

			pushMatrix();
			translate(x, y, z);
			rotate(vx, vy, vz, angle);

			for (i=0; i<polySet->getCoordIndexSetSize(); i++)
			{
				IndexPtr index = polySet->getCoordIndex(i);
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
