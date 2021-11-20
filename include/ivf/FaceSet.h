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

#pragma once

#include <ivf/Shape.h>

// Obsolete use CTriSet, CQuadSet or CPolySet instead.

namespace ivf {

IvfSmartPointer(CFaceSet);

class IVF_API CFaceSet : public CShape {
public:
	CFaceSet();

	virtual ~CFaceSet();

	IvfClassInfo("CIvfFaceSet",CShape);

	void setCoordSize(long coordSize);
	long getCoordSize();

	void setCoord(long n, double x, double y, double z);
	void getCoord(long n, double& x, double& y, double& z);

	void setColor(long n, float r, float g, float b);
	void getColor(long n, float& r, float& g, float& b);

	void setCoordIndexSize(long indexSize);
	long getCoordIndexSize();

	void setTextureCoordSize(long size);
	long getTextureCoordSize();

	void setCoordIndex(long n, long i1, long i2, long i3, long i4);
	void getCoordIndex(long n, long& i1, long& i2, long& i3, long& i4);

	void setTextureCoord(long n, double s, double t);
	void getTextureCoord(long n, double& s, double& t);

	void setTextureCoordIndex(long n, long i1, long i2, long i3, long i4);
	void getTextureCoordIndex(long n, long& i1, long& i2, long& i3, long& i4);

	void setUseColor(bool flag);
	bool getUseColor();

	void setUseTexture(bool flag);
	bool getUseTexture();


protected:
	virtual void doCreateGeometry();
private:
	long m_nCoordIndex;
	long m_nFaces;
	long m_nCoords;
	long m_nTextureCoords;
	double(*m_coords)[3];
	double(*m_textureCoords)[2];
	float(*m_colors)[3];
	double(*m_normals)[3];
	long(*m_coordIndex)[4];
	long(*m_textureCoordIndex)[4];
	bool m_useColor;
	bool m_useTexture;
};

}