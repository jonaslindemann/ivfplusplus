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

#ifndef _CIvfMultiply_h_
#define _CIvfMultiply_h_

#include <ivf/IvfShape.h>

IvfSmartPointer(CIvfMultiply);

class IVF_API CIvfMultiply: public CIvfShape {
private:
	CIvfShapePtr	m_shape;
	double			m_offsets[3];
	int				m_repeat[3];
	int				m_startIndices[3];
public:
	CIvfMultiply();

	IvfClassInfo("CIvfMultiply", CIvfShape);

	virtual void refresh();

	void setShape(CIvfShape* shape);
	CIvfShape* getShape();

	void setOffsets(double dx, double dy, double dz);
	void getOffsets(double &dx, double &dy, double &dz);

	void setRepeat(int x, int y, int z);
	void getRepeat(int &x, int &y, int &z);

	void setStartIndices(int i, int j, int k);
	void getStartIndices(int &i, int &j, int &k);
protected:
	virtual void doCreateGeometry();
};

#endif
