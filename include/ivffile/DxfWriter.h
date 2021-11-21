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

#include <ivffile/FileWriter.h>
#include <ivfmath/MatrixStack.h>

namespace ivf {

IvfSmartPointer(CDxfWriter);

/**
* File export base class
*/
class IVFFILE_API CDxfWriter : public CFileWriter {
private:
	std::fstream m_file;
	CMatrixStackPtr m_matrixStack;
	std::string m_currentLayer;
	int m_currentColor;
protected:
	void processShape(CShape* shape);

	void dxfTag(int number, const std::string& value);
	void dxfInt(int number, int value);
	void dxfFloat(int number, double value);
	void dxfBeginSection(const std::string& value);
	void dxfEndSection();
	void dxfBeginTables();
	void dxfEndTables();
	void dxfBeginTable(const std::string& value);
	void dxfEndTable();
	void dxfBeginLayer();
	void dxfEndLayer();
	void dxfBeginEntitites();
	void dxfEndEntitites();
	void dxfColor(int number);
	void dxfLineType(const std::string& name);
	void dxfLine();
	void dxfLayer(const std::string& value);
	void dxfFirstPoint(double x, double y, double z);
	void dxfSecondPoint(double x, double y, double z);
	void dxfThirdPoint(double x, double y, double z);
	void dxfFourthPoint(double x, double y, double z);
	void dxfPolyline();
	void dxfPolylineIntro();
	void dxfPolylineFlag(int flag);
	void dxfVertex();
	void dxfEndSeq();
	void dxfSolid();
	void dxfEndOfFile();
	void dxf3DFace();

	void quad(
			double x1, double y1, double z1,
			double x2, double y2, double z2,
			double x3, double y3, double z3,
			double x4, double y4, double z4
		);

	void tri(
			double x1, double y1, double z1,
			double x2, double y2, double z2,
			double x3, double y3, double z3
		);

	void pushMatrix();
	void translate(double dx, double dy, double dz);
	void rotate(double vx, double vy, double vz, double theta);
	void popMatrix();

	void transform(double &x, double &y, double &z);

	void setCurrentLayer(const std::string& layerName);
	void setCurrentColor(int color);
public:
	CDxfWriter();
	virtual ~CDxfWriter();

	void write();

	IvfClassInfo("CDxfWriter",CFileWriter);
	IvfStdFactory(CDxfWriter);

};

}