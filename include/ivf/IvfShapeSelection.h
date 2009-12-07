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

#ifndef _CIvfShapeSelection_h_
#define _CIvfShapeSelection_h_

#include <ivf/IvfShape.h>
#include <ivf/IvfMaterial.h>
#include <ivf/IvfComposite.h>

#include <vector>

typedef std::vector<CIvfShapePtr> CIvfShapeSelectionVector;
typedef std::vector<CIvfShapePtr>::iterator CIvfShapeSelectionVectorIterator;

IvfSmartPointer(CIvfShapeSelection);

class IVF_API CIvfShapeSelection : public CIvfShape {
private:
	CIvfShapeSelectionVector m_selection;	
	CIvfMaterialPtr m_hlMaterial;
	std::set<CIvfShape*> m_selectionSet;
	double m_offset[3];
public:
	void setHighlightMaterial(CIvfMaterial* material);
	CIvfShapeSelection();
	virtual ~CIvfShapeSelection();

	IvfClassInfo("CIvfShapeSelection",CIvfShape);

	void add(CIvfShape* shape);
	void clear();
	CIvfShape* getShape(int idx);
	int getSize();
	void remove(CIvfShape* shape);

	bool contains(CIvfShape* shape);
	void assignFrom(CIvfShapeSelection* selection);
	void moveShapes(double dx, double dy, double dz);
	void addTo(CIvfComposite* composite);
	void setOffset(double dx, double dy, double dz);

	void getSelection(CIvfShapeSelectionVector& vector);
protected:
	virtual void doCreateGeometry();
};

#endif 
