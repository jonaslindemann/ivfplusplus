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

#ifndef _CIvfDxfWriter_h_
#define _CIvfDxfWriter_h_

#include <ivffile/IvfFileWriter.h>
#include <ivfmath/IvfMatrixStack.h>

IvfSmartPointer(CIvfDxfWriter);

/**
* File export base class
*/
class IVFFILE_API CIvfDxfWriter : public CIvfFileWriter {
private:
	std::fstream m_file;
	CIvfMatrixStackPtr m_matrixStack;
	std::string m_currentLayer;
	int m_currentColor;
protected:
	void processShape(CIvfShape* shape);
	
	void dxfTag(int number, const char* value);
	void dxfInt(int number, int value);
	void dxfFloat(int number, double value);
	void dxfBeginSection(const char* value);
	void dxfEndSection();
	void dxfBeginTables();
	void dxfEndTables();
	void dxfBeginTable(const char* value);
	void dxfEndTable();
	void dxfBeginLayer();
	void dxfEndLayer();
	void dxfBeginEntitites();
	void dxfEndEntitites();
	void dxfColor(int number);
	void dxfLineType(const char* name);
	void dxfLine();
	void dxfLayer(const char* value);
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
	
	void setCurrentLayer(const char* layerName);
	void setCurrentColor(int color);
public:
	CIvfDxfWriter();
	virtual ~CIvfDxfWriter();
	
	void write();
	
	IvfClassInfo("CIvfDxfWriter",CIvfFileWriter);
};

#endif 
