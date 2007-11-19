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

#include <ivf/IvfShapeSelection.h>

#include <ivf/IvfSolidLine.h>

CIvfShapeSelection::CIvfShapeSelection()
{
	m_hlMaterial = new CIvfMaterial();
	m_hlMaterial->setDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_hlMaterial->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_hlMaterial->setAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);

	m_offset[0] = 0.0;
	m_offset[1] = 0.0;
	m_offset[2] = 0.0;
}

CIvfShapeSelection::~CIvfShapeSelection()
{

}

void CIvfShapeSelection::createGeometry()
{
	CIvfShapeSelectionVectorIterator it;

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);


	for (it=m_selection.begin(); it!=m_selection.end(); it++)
	{
		CIvfShapePtr shape = *it;


		if (!shape->isClass("CIvfSolidLine"))
		{
			glPushMatrix();
			glTranslated(m_offset[0], m_offset[1], m_offset[2]);
			CIvfMaterialPtr oldMat = shape->getMaterial();
			shape->setMaterial(m_hlMaterial);
			shape->setScale(1.4, 1.4, 1.4);
			shape->render();
			shape->setScale(1.0, 1.0, 1.0);
			shape->setMaterial(oldMat);
			glPopMatrix();
		}
		else
		{
			CIvfShape* temp = shape;
			CIvfSolidLinePtr solidLine = (CIvfSolidLine*) temp;

			double oldRadius = solidLine->getRadius();
			solidLine->setRadius(oldRadius*1.4);

			CIvfMaterialPtr oldMat = shape->getMaterial();
			shape->setMaterial(m_hlMaterial);
			shape->render();
			shape->setMaterial(oldMat);

			solidLine->setRadius(oldRadius);
		}
	}


	glDisable(GL_BLEND);
}

void CIvfShapeSelection::add(CIvfShape *shape)
{
	m_selectionSet.insert(shape);
	if (m_selectionSet.size()>m_selection.size())
		m_selection.push_back(CIvfShapePtr(shape));
}

void CIvfShapeSelection::clear()
{
	m_selection.clear();
	m_selectionSet.clear();
}

int CIvfShapeSelection::getSize()
{
	return m_selection.size();
}

CIvfShape* CIvfShapeSelection::getShape(int idx)
{
	if ((idx>=0)&&(idx<(int)m_selection.size()))
		return m_selection[idx];
	else
		return NULL;
}

void CIvfShapeSelection::getSelection(CIvfShapeSelectionVector &vec)
{
	CIvfShapeSelectionVectorIterator it;

	for (it=m_selection.begin(); it!=m_selection.end(); it++)
	{
		CIvfShapePtr shape = *it;
		vec.push_back(shape);
	}
}

bool CIvfShapeSelection::contains(CIvfShape *shape)
{
	std::set<CIvfShape*>::iterator it;

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

void CIvfShapeSelection::addTo(CIvfComposite *composite)
{
	CIvfShapeSelectionVectorIterator it;

	for (it=m_selection.begin(); it!=m_selection.end(); it++)
	{
		CIvfShapePtr shape = *it;
		composite->addChild(shape);
	}
}

void CIvfShapeSelection::moveShapes(double dx, double dy, double dz)
{
	CIvfShapeSelectionVectorIterator it;
	double x, y, z;

	for (it=m_selection.begin(); it!=m_selection.end(); it++)
	{
		CIvfShapePtr shape = *it;

		shape->getPosition(x, y, z);
		x+=dx;
		y+=dy;
		z+=dz;
		shape->setPosition(x, y, z);
	}
}

void CIvfShapeSelection::assignFrom(CIvfShapeSelection *selection)
{
	clear();

	int i;

	for (i=0; i<selection->getSize(); i++)
	{
		CIvfShape* shape = selection->getShape(i);
		add(shape);
	}
}

void CIvfShapeSelection::setOffset(double dx, double dy, double dz)
{
	m_offset[0] = dx;
	m_offset[1] = dy;
	m_offset[2] = dz;
}

void CIvfShapeSelection::remove(CIvfShape *shape)
{
	std::set<CIvfShape*>::iterator sit;
	std::vector<CIvfShapePtr>::iterator vit;

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

void CIvfShapeSelection::setHighlightMaterial(CIvfMaterial *material)
{
	m_hlMaterial = material;
}
