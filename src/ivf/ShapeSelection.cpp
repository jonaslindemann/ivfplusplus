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

#include <ivf/ShapeSelection.h>
#include <ivf/SolidLine.h>

using namespace ivf;

ShapeSelection::ShapeSelection()
{
	m_hlMaterial = new Material();
	m_hlMaterial->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_hlMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_hlMaterial->setAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);

	m_offset[0] = 0.0;
	m_offset[1] = 0.0;
	m_offset[2] = 0.0;
}

ShapeSelection::~ShapeSelection()
{

}

void ShapeSelection::doCreateGeometry()
{
	ShapeSelectionVectorIterator it;

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);


	for (it=m_selection.begin(); it!=m_selection.end(); it++)
	{
		ShapePtr shape = *it;


		if (!shape->isClass("SolidLine"))
		{
			glPushMatrix();
			glTranslated(m_offset[0], m_offset[1], m_offset[2]);
			MaterialPtr oldMat = shape->getMaterial();
			shape->setMaterial(m_hlMaterial);
			shape->setScale(1.4, 1.4, 1.4);
			shape->render();
			shape->setScale(1.0, 1.0, 1.0);
			shape->setMaterial(oldMat);
			glPopMatrix();
		}
		else
		{
			Shape* temp = shape;
			SolidLinePtr solidLine = (SolidLine*) temp;

			double oldRadius = solidLine->getRadius();
			solidLine->setRadius(oldRadius*1.4);

			MaterialPtr oldMat = shape->getMaterial();
			shape->setMaterial(m_hlMaterial);
			shape->render();
			shape->setMaterial(oldMat);

			solidLine->setRadius(oldRadius);
		}
	}


	glDisable(GL_BLEND);
}

void ShapeSelection::add(Shape *shape)
{
	m_selectionSet.insert(shape);
	if (m_selectionSet.size()>m_selection.size())
		m_selection.push_back(ShapePtr(shape));
}

void ShapeSelection::clear()
{
	m_selection.clear();
	m_selectionSet.clear();
}

int ShapeSelection::getSize()
{
	return static_cast<int>(m_selection.size());
}

Shape* ShapeSelection::getShape(int idx)
{
	if ((idx>=0)&&(idx<(int)m_selection.size()))
		return m_selection[idx];
	else
		return nullptr;
}

void ShapeSelection::getSelection(ShapeSelectionVector &vec)
{
	ShapeSelectionVectorIterator it;

	for (it=m_selection.begin(); it!=m_selection.end(); it++)
	{
		ShapePtr shape = *it;
		vec.push_back(shape);
	}
}

bool ShapeSelection::contains(Shape *shape)
{
	std::set<Shape*>::iterator it;

	it = m_selectionSet.find(shape);
	return (it != m_selectionSet.end());

	/*
	CIvfShapeSelectionVectorIterator it;

	for (it=m_selection.begin(); it!=m_selection.end(); it++)
	{
		CIvfShapePtr selectedShape = *it;

		if (shape==selectedShape)
			return true;
	}

	return false;
	*/
}

void ShapeSelection::addTo(Composite *composite)
{
	ShapeSelectionVectorIterator it;

	for (it=m_selection.begin(); it!=m_selection.end(); it++)
	{
		ShapePtr shape = *it;
		composite->addChild(shape);
	}
}

void ShapeSelection::moveShapes(double dx, double dy, double dz)
{
	ShapeSelectionVectorIterator it;
	double x, y, z;

	for (it=m_selection.begin(); it!=m_selection.end(); it++)
	{
		ShapePtr shape = *it;

		shape->getPosition(x, y, z);
		x+=dx;
		y+=dy;
		z+=dz;
		shape->setPosition(x, y, z);
	}
}

void ShapeSelection::assignFrom(ShapeSelection *selection)
{
	clear();

	int i;

	for (i=0; i<selection->getSize(); i++)
	{
		Shape* shape = selection->getShape(i);
		add(shape);
	}
}

void ShapeSelection::setOffset(double dx, double dy, double dz)
{
	m_offset[0] = dx;
	m_offset[1] = dy;
	m_offset[2] = dz;
}

void ShapeSelection::remove(Shape *shape)
{
	std::set<Shape*>::iterator sit;
	std::vector<ShapePtr>::iterator vit;

	sit = m_selectionSet.find(shape);

	if (sit!=m_selectionSet.end())
		m_selectionSet.erase(sit);

	for (vit=m_selection.begin(); vit!=m_selection.end(); vit++)
	{
		if (*vit==shape)
		{
			m_selection.erase(vit);
			return;
		}
	}
}

void ShapeSelection::setHighlightMaterial(Material *material)
{
	m_hlMaterial = material;
}
