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

#include <ivf/Shape.h>
#include <ivf/Material.h>
#include <ivf/Composite.h>

#include <vector>

namespace ivf {

typedef std::vector<ShapePtr> ShapeSelectionVector;
typedef std::vector<ShapePtr>::iterator ShapeSelectionVectorIterator;

IvfSmartPointer(ShapeSelection);

class IVF_API ShapeSelection : public Shape {
private:
	ShapeSelectionVector m_selection;	
	MaterialPtr m_hlMaterial;
	std::set<Shape*> m_selectionSet;
	double m_offset[3];
public:
	void setHighlightMaterial(Material* material);
	ShapeSelection();
	virtual ~ShapeSelection();

	IvfClassInfo("ShapeSelection",Shape);
	IvfStdFactory(ShapeSelection);

	void add(Shape* shape);
	void clear();
	Shape* getShape(int idx);
	int getSize();
	void remove(Shape* shape);

	bool contains(Shape* shape);
	void assignFrom(ShapeSelection* selection);
	void moveShapes(double dx, double dy, double dz);
	void addTo(Composite* composite);
	void setOffset(double dx, double dy, double dz);

	void getSelection(ShapeSelectionVector& vector);
protected:
	virtual void doCreateGeometry();
};

}